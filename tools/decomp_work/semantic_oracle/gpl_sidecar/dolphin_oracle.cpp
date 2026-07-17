// SPDX-License-Identifier: GPL-2.0-or-later
//
// This executable links the GPL RecompCore/Dolphin fork pinned by
// ModernGekko.  It is intentionally a separate process from the decompilation
// campaign's MIT Python tooling.

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "Common/Config/Config.h"
#include "Common/MsgHandler.h"
#include "Core/Config/MainSettings.h"
#include "Core/Config/ConfigManager.h"
#include "Core/Core.h"
#include "Core/CoreTiming.h"
#include "Core/HW/Memmap.h"
#include "Core/PowerPC/Interpreter/Interpreter.h"
#include "Core/PowerPC/MMU.h"
#include "Core/PowerPC/PowerPC.h"
#include "Core/System.h"
#include "UICommon/UICommon.h"
#include "picojson.h"

#if !defined(ORACLE_MODERNGEKKO_COMMIT) || !defined(ORACLE_RECOMPCORE_COMMIT) || \
    !defined(ORACLE_DOLRECOMP_COMMIT)
#error "The semantic sidecar must be built with its reviewed dependency pins"
#endif

namespace
{
constexpr std::uint32_t kProtocol = 1;
constexpr std::uint32_t kMem1Base = 0x80000000u;
constexpr std::uint32_t kDefaultReturnPc = 0x817ff000u;
constexpr std::uint64_t kMaximumInstructions = 1'000'000;
constexpr std::size_t kCodeSandboxBytes = 4096;
constexpr std::size_t kMaximumCodeBytes = kCodeSandboxBytes;
constexpr std::size_t kMaximumPatchBytes = 1u << 20;
constexpr std::size_t kMaximumObserveBytes = 1u << 20;
constexpr std::size_t kMaximumChangeSpans = 32;
constexpr std::size_t kMaximumChangeBytes = 512;

using JsonObject = picojson::object;
using JsonArray = picojson::array;

bool s_alert_seen = false;

bool AlertHandler(const char*, const char*, bool, Common::MsgType)
{
  s_alert_seen = true;
  return true;
}

[[noreturn]] void Fail(const std::string& message)
{
  throw std::runtime_error(message);
}

const picojson::value& Required(const JsonObject& object, const std::string& key)
{
  const auto it = object.find(key);
  if (it == object.end())
    Fail("missing field: " + key);
  return it->second;
}

const JsonObject& AsObject(const picojson::value& value, const std::string& field)
{
  if (!value.is<JsonObject>())
    Fail(field + " must be an object");
  return value.get<JsonObject>();
}

const JsonArray& AsArray(const picojson::value& value, const std::string& field)
{
  if (!value.is<JsonArray>())
    Fail(field + " must be an array");
  return value.get<JsonArray>();
}

std::string AsString(const picojson::value& value, const std::string& field)
{
  if (!value.is<std::string>())
    Fail(field + " must be a string");
  return value.get<std::string>();
}

std::uint64_t AsUnsigned(const picojson::value& value, const std::string& field,
                         std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max())
{
  std::uint64_t result = 0;
  if (value.is<std::string>())
  {
    std::string_view text = value.get<std::string>();
    int base = 10;
    if (text.starts_with("0x") || text.starts_with("0X"))
    {
      text.remove_prefix(2);
      base = 16;
    }
    if (text.empty())
      Fail(field + " is empty");
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), result, base);
    if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size())
      Fail(field + " is not an unsigned integer");
  }
  else if (value.is<double>())
  {
    const double number = value.get<double>();
    if (number < 0 || number != static_cast<double>(static_cast<std::uint64_t>(number)))
      Fail(field + " is not an unsigned integer");
    result = static_cast<std::uint64_t>(number);
  }
  else
  {
    Fail(field + " must be an integer or hexadecimal string");
  }
  if (result > maximum)
    Fail(field + " exceeds its limit");
  return result;
}

std::uint32_t AsU32(const picojson::value& value, const std::string& field)
{
  return static_cast<std::uint32_t>(AsUnsigned(value, field, 0xffffffffu));
}

std::string Hex32(std::uint32_t value)
{
  std::ostringstream out;
  out << "0x" << std::hex << std::setfill('0') << std::setw(8) << value;
  return out.str();
}

std::string Hex64(std::uint64_t value)
{
  std::ostringstream out;
  out << "0x" << std::hex << std::setfill('0') << std::setw(16) << value;
  return out.str();
}

unsigned HexNibble(char character)
{
  if (character >= '0' && character <= '9')
    return static_cast<unsigned>(character - '0');
  if (character >= 'a' && character <= 'f')
    return static_cast<unsigned>(character - 'a' + 10);
  if (character >= 'A' && character <= 'F')
    return static_cast<unsigned>(character - 'A' + 10);
  Fail("invalid hexadecimal data");
}

std::vector<std::uint8_t> DecodeHex(const std::string& text, const std::string& field,
                                    std::size_t maximum)
{
  if ((text.size() & 1u) != 0)
    Fail(field + " must contain complete bytes");
  if (text.size() / 2 > maximum)
    Fail(field + " exceeds its byte limit");
  std::vector<std::uint8_t> bytes(text.size() / 2);
  for (std::size_t i = 0; i < bytes.size(); ++i)
    bytes[i] = static_cast<std::uint8_t>((HexNibble(text[2 * i]) << 4) | HexNibble(text[2 * i + 1]));
  return bytes;
}

std::string EncodeHex(const std::uint8_t* bytes, std::size_t size)
{
  static constexpr char digits[] = "0123456789abcdef";
  std::string result(size * 2, '0');
  for (std::size_t i = 0; i < size; ++i)
  {
    result[2 * i] = digits[bytes[i] >> 4];
    result[2 * i + 1] = digits[bytes[i] & 0xf];
  }
  return result;
}

struct MemoryPatch
{
  std::uint32_t address = 0;
  std::vector<std::uint8_t> bytes;
};

struct MemoryObservation
{
  std::uint32_t address = 0;
  std::size_t size = 0;
};

struct Fixture
{
  std::string id;
  std::array<std::uint32_t, 32> gpr{};
  std::uint32_t lr = kDefaultReturnPc;
  std::uint32_t ctr = 0;
  std::uint32_t cr = 0;
  std::uint32_t xer = 0;
  std::vector<MemoryPatch> memory;
  std::vector<unsigned> observe_gpr;
  std::vector<MemoryObservation> observe_memory;
};

struct Request
{
  std::string function;
  std::uint32_t entry_pc = 0;
  std::uint64_t max_instructions = 0;
  std::vector<std::uint8_t> code;
  std::vector<Fixture> fixtures;
};

class DolphinScope final
{
public:
  explicit DolphinScope(const std::filesystem::path& user_directory)
      : m_system(Core::System::GetInstance())
  {
    std::filesystem::create_directories(user_directory);
    Core::DeclareAsCPUThread();
    m_cpu_thread = true;
    UICommon::SetUserDirectory(user_directory.string());
    Config::Init();
    m_config = true;
    SConfig::Init();
    m_sconfig = true;
    m_system.Initialize();
    m_system.GetMemory().Init();
    m_memory = true;
    m_system.GetCoreTiming().Init();
    m_timing = true;
    m_system.GetPowerPC().Init(PowerPC::CPUCore::Interpreter);
    m_power_pc = true;
  }

  ~DolphinScope()
  {
    if (m_power_pc)
      m_system.GetPowerPC().Shutdown();
    if (m_timing)
      m_system.GetCoreTiming().Shutdown();
    if (m_memory)
      m_system.GetMemory().Shutdown();
    if (m_sconfig)
      SConfig::Shutdown();
    if (m_config)
      Config::Shutdown();
    if (m_cpu_thread)
      Core::UndeclareAsCPUThread();
  }

  Core::System& System() { return m_system; }

private:
  Core::System& m_system;
  bool m_cpu_thread = false;
  bool m_config = false;
  bool m_sconfig = false;
  bool m_memory = false;
  bool m_timing = false;
  bool m_power_pc = false;
};

std::uint8_t* CheckedRam(Core::System& system, std::uint32_t address, std::size_t size,
                         const std::string& field)
{
  auto& memory = system.GetMemory();
  const std::uint64_t begin = address;
  const std::uint64_t end = begin + size;
  const std::uint64_t ram_end = static_cast<std::uint64_t>(kMem1Base) + memory.GetRamSizeReal();
  if (begin < kMem1Base || end < begin || end > ram_end)
    Fail(field + " is outside MEM1");
  return memory.GetRAM() + (address - kMem1Base);
}

Request ParseRequest(const picojson::value& root)
{
  const auto& object = AsObject(root, "root");
  if (AsUnsigned(Required(object, "schema_version"), "schema_version", kProtocol) != kProtocol)
    Fail("unsupported schema_version");
  const auto& function = AsObject(Required(object, "function"), "function");

  Request request;
  request.function = AsString(Required(function, "name"), "function.name");
  request.entry_pc = AsU32(Required(function, "entry_pc"), "function.entry_pc");
  request.max_instructions = AsUnsigned(Required(function, "max_instructions"),
                                        "function.max_instructions", kMaximumInstructions);
  request.code = DecodeHex(AsString(Required(function, "code_hex"), "function.code_hex"),
                           "function.code_hex", kMaximumCodeBytes);
  if (request.code.empty() || (request.code.size() & 3u) != 0)
    Fail("function.code_hex must contain aligned PowerPC instructions");
  if ((request.entry_pc & 3u) != 0)
    Fail("function.entry_pc must be aligned");
  if (request.max_instructions == 0)
    Fail("function.max_instructions must be nonzero");

  for (const auto& fixture_value : AsArray(Required(object, "fixtures"), "fixtures"))
  {
    const auto& fixture_object = AsObject(fixture_value, "fixture");
    Fixture fixture;
    fixture.id = AsString(Required(fixture_object, "id"), "fixture.id");
    const auto& initial = AsObject(Required(fixture_object, "initial"), "fixture.initial");
    const auto& gpr = AsObject(Required(initial, "gpr"), "fixture.initial.gpr");
    for (const auto& [index_text, value] : gpr)
    {
      unsigned index = 0;
      const auto parsed = std::from_chars(index_text.data(), index_text.data() + index_text.size(), index);
      if (parsed.ec != std::errc{} || parsed.ptr != index_text.data() + index_text.size() || index >= 32)
        Fail("fixture.initial.gpr contains an invalid register index");
      fixture.gpr[index] = AsU32(value, "fixture.initial.gpr");
    }
    if (const auto it = initial.find("lr"); it != initial.end())
      fixture.lr = AsU32(it->second, "fixture.initial.lr");
    if (const auto it = initial.find("ctr"); it != initial.end())
      fixture.ctr = AsU32(it->second, "fixture.initial.ctr");
    if (const auto it = initial.find("cr"); it != initial.end())
      fixture.cr = AsU32(it->second, "fixture.initial.cr");
    if (const auto it = initial.find("xer"); it != initial.end())
      fixture.xer = AsU32(it->second, "fixture.initial.xer");
    if ((fixture.lr & 3u) != 0)
      Fail("fixture.initial.lr must be aligned");

    if (const auto it = initial.find("memory"); it != initial.end())
    {
      for (const auto& patch_value : AsArray(it->second, "fixture.initial.memory"))
      {
        const auto& patch_object = AsObject(patch_value, "memory patch");
        MemoryPatch patch;
        patch.address = AsU32(Required(patch_object, "address"), "memory patch address");
        patch.bytes = DecodeHex(AsString(Required(patch_object, "data_hex"), "memory patch data_hex"),
                                "memory patch data_hex", kMaximumPatchBytes);
        fixture.memory.emplace_back(std::move(patch));
      }
    }

    const auto& observe = AsObject(Required(fixture_object, "observe"), "fixture.observe");
    for (const auto& register_value : AsArray(Required(observe, "gpr"), "fixture.observe.gpr"))
      fixture.observe_gpr.push_back(static_cast<unsigned>(
          AsUnsigned(register_value, "fixture.observe.gpr", 31)));
    for (const auto& observation_value :
         AsArray(Required(observe, "memory"), "fixture.observe.memory"))
    {
      const auto& observation_object = AsObject(observation_value, "memory observation");
      MemoryObservation observation;
      observation.address =
          AsU32(Required(observation_object, "address"), "memory observation address");
      observation.size = static_cast<std::size_t>(AsUnsigned(
          Required(observation_object, "size"), "memory observation size", kMaximumObserveBytes));
      fixture.observe_memory.emplace_back(observation);
    }
    request.fixtures.emplace_back(std::move(fixture));
  }
  if (request.fixtures.empty())
    Fail("fixtures must not be empty");
  return request;
}

picojson::value RunFixture(Core::System& system, const Request& request, const Fixture& fixture)
{
  auto& memory = system.GetMemory();
  auto& power_pc = system.GetPowerPC();
  auto& ppc = system.GetPPCState();

  memory.Clear();
  auto* const code_sandbox =
      CheckedRam(system, request.entry_pc, kCodeSandboxBytes, "function code sandbox");
  const std::uint64_t sandbox_end =
      static_cast<std::uint64_t>(request.entry_pc) + kCodeSandboxBytes;
  for (const MemoryPatch& patch : fixture.memory)
  {
    const std::uint64_t patch_end = static_cast<std::uint64_t>(patch.address) + patch.bytes.size();
    if (patch.address < sandbox_end && patch_end > request.entry_pc)
      Fail("memory patch overlaps function code sandbox");
    std::copy(patch.bytes.begin(), patch.bytes.end(),
              CheckedRam(system, patch.address, patch.bytes.size(), "memory patch"));
  }
  std::copy(request.code.begin(), request.code.end(), code_sandbox);
  const std::size_t ram_size = memory.GetRamSizeReal();
  // Snapshot after installing code.  Comparisons ignore the same fixed code
  // sandbox for reference and candidate, so different legal .text lengths do
  // not perturb the whole-RAM digest.
  const std::vector<std::uint8_t> initial_ram(memory.GetRAM(), memory.GetRAM() + ram_size);

  power_pc.Reset();
  std::copy(fixture.gpr.begin(), fixture.gpr.end(), ppc.gpr);
  ppc.pc = request.entry_pc;
  ppc.npc = request.entry_pc + 4;
  ppc.spr[SPR_LR] = fixture.lr;
  ppc.spr[SPR_CTR] = fixture.ctr;
  ppc.cr.Set(fixture.cr);
  ppc.SetXER(UReg_XER{fixture.xer});
  // Fixed GC retail virtual-memory profile from Dolphin's CBoot::SetupBAT and
  // CBoot::SetupMSR.  Without it, 0x80000000 MEM1 addresses fault instead of
  // mapping to physical RAM.
  ppc.spr[SPR_IBAT0U] = 0x80001fffu;
  ppc.spr[SPR_IBAT0L] = 0x00000002u;
  ppc.spr[SPR_DBAT0U] = 0x80001fffu;
  ppc.spr[SPR_DBAT0L] = 0x00000002u;
  ppc.spr[SPR_DBAT1U] = 0xc0001fffu;
  ppc.spr[SPR_DBAT1L] = 0x0000002au;
  system.GetMMU().IBATUpdated();
  system.GetMMU().DBATUpdated();
  ppc.msr.Hex = 0x00002032u;
  ppc.Exceptions = 0;
  power_pc.MSRUpdated();
  s_alert_seen = false;

  std::string status = "step_limit";
  std::uint64_t instructions = 0;
  const std::uint64_t code_end = static_cast<std::uint64_t>(request.entry_pc) + request.code.size();
  while (instructions < request.max_instructions)
  {
    if (ppc.pc == fixture.lr)
    {
      status = "returned";
      break;
    }
    if (ppc.pc < request.entry_pc || ppc.pc >= code_end || (ppc.pc & 3u) != 0)
    {
      status = "pc_out_of_range";
      break;
    }
    system.GetInterpreter().SingleStepInner();
    ++instructions;
    if (s_alert_seen)
    {
      status = "alert";
      break;
    }
    if (ppc.Exceptions != 0)
    {
      status = "exception";
      break;
    }
  }
  if (status == "step_limit" && ppc.pc == fixture.lr && ppc.Exceptions == 0)
    status = "returned";
  const std::size_t sandbox_begin = request.entry_pc - kMem1Base;
  const std::size_t sandbox_end_offset = sandbox_begin + kCodeSandboxBytes;
  if (!std::equal(initial_ram.begin() + sandbox_begin, initial_ram.begin() + sandbox_end_offset,
                  code_sandbox))
    status = "code_modified";

  JsonObject final;
  JsonObject registers;
  for (const unsigned index : fixture.observe_gpr)
    registers.emplace(std::to_string(index), picojson::value(Hex32(ppc.gpr[index])));
  final.emplace("gpr", picojson::value(std::move(registers)));
  final.emplace("pc", picojson::value(Hex32(ppc.pc)));
  final.emplace("lr", picojson::value(Hex32(ppc.spr[SPR_LR])));
  final.emplace("ctr", picojson::value(Hex32(ppc.spr[SPR_CTR])));
  final.emplace("cr", picojson::value(Hex32(ppc.cr.Get())));
  final.emplace("xer", picojson::value(Hex32(ppc.GetXER().Hex)));

  JsonArray observed_memory;
  for (const MemoryObservation& observation : fixture.observe_memory)
  {
    const auto* data = CheckedRam(system, observation.address, observation.size, "memory observation");
    JsonObject item;
    item.emplace("address", picojson::value(Hex32(observation.address)));
    item.emplace("data_hex", picojson::value(EncodeHex(data, observation.size)));
    observed_memory.emplace_back(std::move(item));
  }
  final.emplace("memory", picojson::value(std::move(observed_memory)));

  std::uint64_t ram_digest = 14695981039346656037ull;
  for (std::size_t offset = 0; offset < ram_size; ++offset)
  {
    if (offset >= sandbox_begin && offset < sandbox_end_offset)
      continue;
    ram_digest ^= memory.GetRAM()[offset];
    ram_digest *= 1099511628211ull;
  }
  std::size_t changed_bytes = 0;
  std::size_t reported_change_bytes = 0;
  bool changes_truncated = false;
  JsonArray ram_changes;
  for (std::size_t offset = 0; offset < ram_size;)
  {
    if (offset >= sandbox_begin && offset < sandbox_end_offset)
    {
      offset = sandbox_end_offset;
      continue;
    }
    const std::uint8_t current = memory.GetRAM()[offset];
    if (current == initial_ram[offset])
    {
      ++offset;
      continue;
    }

    const std::size_t span_begin = offset;
    while (offset < ram_size && !(offset >= sandbox_begin && offset < sandbox_end_offset) &&
           memory.GetRAM()[offset] != initial_ram[offset])
    {
      ++changed_bytes;
      ++offset;
    }
    const std::size_t span_size = offset - span_begin;
    if (ram_changes.size() < kMaximumChangeSpans &&
        reported_change_bytes + span_size <= kMaximumChangeBytes)
    {
      JsonObject change;
      change.emplace("address",
                     picojson::value(Hex32(kMem1Base + static_cast<std::uint32_t>(span_begin))));
      change.emplace("before_hex",
                     picojson::value(EncodeHex(initial_ram.data() + span_begin, span_size)));
      change.emplace("after_hex",
                     picojson::value(EncodeHex(memory.GetRAM() + span_begin, span_size)));
      ram_changes.emplace_back(std::move(change));
      reported_change_bytes += span_size;
    }
    else
    {
      changes_truncated = true;
    }
  }

  JsonObject result;
  result.emplace("id", picojson::value(fixture.id));
  result.emplace("status", picojson::value(status));
  result.emplace("instructions", picojson::value(static_cast<double>(instructions)));
  result.emplace("final", picojson::value(std::move(final)));
  result.emplace("ram_digest", picojson::value(Hex64(ram_digest)));
  result.emplace("ram_changed_bytes", picojson::value(static_cast<double>(changed_bytes)));
  result.emplace("ram_changes_truncated", picojson::value(changes_truncated));
  result.emplace("ram_changes", picojson::value(std::move(ram_changes)));
  return picojson::value(std::move(result));
}

picojson::value RunRequest(const Request& request, const std::filesystem::path& user_directory)
{
  DolphinScope dolphin(user_directory);
  JsonArray results;
  for (const Fixture& fixture : request.fixtures)
    results.emplace_back(RunFixture(dolphin.System(), request, fixture));

  JsonObject root;
  root.emplace("schema_version", picojson::value(static_cast<double>(kProtocol)));
  root.emplace("engine", picojson::value("dolphin-interpreter-from-moderngekko-tree"));
  root.emplace("code_sandbox_bytes", picojson::value(static_cast<double>(kCodeSandboxBytes)));
  JsonObject provenance;
  provenance.emplace("ModernGekko", picojson::value(ORACLE_MODERNGEKKO_COMMIT));
  provenance.emplace("RecompCore", picojson::value(ORACLE_RECOMPCORE_COMMIT));
  provenance.emplace("DolRecomp", picojson::value(ORACLE_DOLRECOMP_COMMIT));
  root.emplace("provenance", picojson::value(std::move(provenance)));
  root.emplace("function", picojson::value(request.function));
  root.emplace("results", picojson::value(std::move(results)));
  return picojson::value(std::move(root));
}

std::string ReadFile(const std::filesystem::path& path)
{
  std::ifstream input(path, std::ios::binary);
  if (!input)
    Fail("cannot open request file");
  return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void WriteFile(const std::filesystem::path& path, const std::string& contents)
{
  std::ofstream output(path, std::ios::binary | std::ios::trunc);
  if (!output)
    Fail("cannot open result file");
  output << contents << '\n';
  if (!output)
    Fail("cannot write result file");
}

}  // namespace

int main(int argc, char** argv)
{
  try
  {
    Common::RegisterMsgAlertHandler(AlertHandler);
    std::filesystem::path request_path;
    std::filesystem::path result_path;
    for (int i = 1; i < argc; ++i)
    {
      const std::string argument = argv[i];
      const auto value = [&](const char* option) {
        if (++i >= argc)
          Fail(std::string(option) + " requires a value");
        return std::filesystem::path(argv[i]);
      };
      if (argument == "--request-file")
        request_path = value("--request-file");
      else if (argument == "--result-file")
        result_path = value("--result-file");
      else
        Fail("unknown argument: " + argument);
    }
    if (request_path.empty() || result_path.empty())
      Fail("usage: moderngekko-dolphin-oracle --request-file FILE --result-file FILE");

    picojson::value root;
    const std::string error = picojson::parse(root, ReadFile(request_path));
    if (!error.empty())
      Fail("invalid request JSON: " + error);
    const Request request = ParseRequest(root);
    const auto user_directory = result_path.parent_path() / "dolphin-user";
    WriteFile(result_path, RunRequest(request, user_directory).serialize());
    return 0;
  }
  catch (const std::exception& error)
  {
    std::cerr << "moderngekko-dolphin-oracle: " << error.what() << '\n';
    return 2;
  }
}
