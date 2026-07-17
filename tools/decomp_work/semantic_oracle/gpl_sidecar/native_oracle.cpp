// SPDX-License-Identifier: GPL-3.0-or-later
//
// This executable links DolRecomp's standalone CPU runtime and two generated
// GC6E01 chunks. It intentionally remains a separate process from the MIT
// decompilation tooling and executes only pinned, original-DOL functions.

#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
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

extern "C"
{
#include "cpu/cpu.h"

void func_800ED5E0(CPUState* ctx);
void func_801315E0(CPUState* ctx);
}

#include "picojson.h"

#if !defined(ORACLE_MODERNGEKKO_COMMIT) || !defined(ORACLE_RECOMPCORE_COMMIT) || \
    !defined(ORACLE_DOLRECOMP_COMMIT) || !defined(ORACLE_GENERATED_TREE_SHA256)
#error "The native semantic sidecar must be built with its reviewed dependency pins"
#endif

namespace
{
constexpr std::uint32_t kProtocol = 1;
constexpr std::uint32_t kMem1Base = 0x80000000u;
constexpr std::uint32_t kSentinelReturnPc = 0x817fff00u;
constexpr std::uint64_t kMaximumDispatches = 1'000'000;
constexpr std::size_t kMaximumFixtures = 10'000;
constexpr std::uintmax_t kMaximumRequestBytes = 64u * 1024u * 1024u;
constexpr std::size_t kCodeSandboxBytes = 4096;
constexpr std::size_t kMaximumPatchBytes = 1u << 20;
constexpr std::size_t kMaximumObserveBytes = 1u << 20;
constexpr std::size_t kMaximumObservationsPerFixture = 64;
constexpr std::size_t kMaximumObservedBytesPerFixture = 4u << 20;
constexpr std::size_t kMaximumObservedBytesPerRequest = 16u << 20;
constexpr std::size_t kMaximumChangeSpans = 32;
constexpr std::size_t kMaximumChangeBytes = 512;
constexpr std::string_view kDolSha1 = "870e8b9693ca780782d80f22a6a4572d8ba9458f";
constexpr std::string_view kGeneratedTreeSha256 = ORACLE_GENERATED_TREE_SHA256;

using JsonObject = picojson::object;
using JsonArray = picojson::array;
using ChunkFunction = void (*)(CPUState*);

struct SupportedFunction
{
  std::string_view name;
  std::uint32_t entry_pc;
  std::uint32_t size;
  std::uint32_t chunk_begin;
  std::uint32_t chunk_end;
  ChunkFunction chunk;
};

// The generated chunks are bound to GC6E01 main.dol by the adjacent generated
// manifest and build attestation. Raw target instructions are intentionally not
// copied into this repository.
constexpr SupportedFunction kSupportedFunctions[] = {
    {"GStextureLockImage", 0x800ef548u, 0x30u, 0x800ed5e0u, 0x800f15e0u, func_800ED5E0},
    {"msgctrlWait", 0x80132454u, 0x78u, 0x801315e0u, 0x801355e0u, func_801315E0},
};

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
    constexpr double kUint64ExclusiveLimit = 18446744073709551616.0;
    if (!std::isfinite(number) || number < 0 || number >= kUint64ExclusiveLimit ||
        std::trunc(number) != number ||
        (maximum != std::numeric_limits<std::uint64_t>::max() &&
         number > static_cast<double>(maximum)))
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
  std::uint32_t lr = kSentinelReturnPc;
  std::uint32_t ctr = 0;
  std::uint32_t cr = 0;
  std::uint32_t xer = 0;
  std::vector<MemoryPatch> memory;
  std::vector<unsigned> observe_gpr;
  std::vector<MemoryObservation> observe_memory;
};

struct Request
{
  const SupportedFunction* function = nullptr;
  std::uint64_t max_dispatches = 0;
  std::vector<std::uint8_t> code;
  std::vector<Fixture> fixtures;
};

const SupportedFunction& FindFunction(std::string_view name, std::uint32_t entry_pc)
{
  for (const SupportedFunction& function : kSupportedFunctions)
  {
    if (function.name == name && function.entry_pc == entry_pc)
      return function;
  }
  Fail("unsupported original function or entry_pc");
}

Request ParseRequest(const picojson::value& root)
{
  const auto& object = AsObject(root, "root");
  if (AsUnsigned(Required(object, "schema_version"), "schema_version", kProtocol) != kProtocol)
    Fail("unsupported schema_version");
  const auto& function_object = AsObject(Required(object, "function"), "function");
  const std::string function_name =
      AsString(Required(function_object, "name"), "function.name");
  const std::uint32_t entry_pc =
      AsU32(Required(function_object, "entry_pc"), "function.entry_pc");
  const SupportedFunction& supported = FindFunction(function_name, entry_pc);

  const auto& original = AsObject(Required(function_object, "original"), "function.original");
  if (AsU32(Required(original, "virtual_address"), "function.original.virtual_address") !=
      supported.entry_pc)
    Fail("function.original.virtual_address does not match the pinned function");
  if (AsUnsigned(Required(original, "size"), "function.original.size", kCodeSandboxBytes) !=
      supported.size)
    Fail("function.original.size does not match the pinned function");
  if (AsString(Required(original, "dol_sha1"), "function.original.dol_sha1") != kDolSha1)
    Fail("function.original.dol_sha1 does not match GC6E01");

  Request request;
  request.function = &supported;
  request.max_dispatches =
      AsUnsigned(Required(function_object, "max_instructions"), "function.max_instructions",
                 kMaximumDispatches);
  if (request.max_dispatches == 0)
    Fail("function.max_instructions must be nonzero");
  request.code =
      DecodeHex(AsString(Required(function_object, "code_hex"), "function.code_hex"),
                "function.code_hex", kCodeSandboxBytes);
  if (request.code.size() != supported.size)
    Fail("function.code_hex length does not match the pinned original function");

  const JsonArray& fixture_values = AsArray(Required(object, "fixtures"), "fixtures");
  if (fixture_values.size() > kMaximumFixtures)
    Fail("fixtures exceeds its limit");
  std::size_t request_observed_bytes = 0;
  for (const auto& fixture_value : fixture_values)
  {
    const auto& fixture_object = AsObject(fixture_value, "fixture");
    Fixture fixture;
    fixture.id = AsString(Required(fixture_object, "id"), "fixture.id");
    const auto& initial = AsObject(Required(fixture_object, "initial"), "fixture.initial");
    const auto& gpr = AsObject(Required(initial, "gpr"), "fixture.initial.gpr");
    for (const auto& [index_text, value] : gpr)
    {
      unsigned index = 0;
      const auto parsed =
          std::from_chars(index_text.data(), index_text.data() + index_text.size(), index);
      if (parsed.ec != std::errc{} || parsed.ptr != index_text.data() + index_text.size() ||
          index >= 32)
        Fail("fixture.initial.gpr contains an invalid register index");
      fixture.gpr[index] = AsU32(value, "fixture.initial.gpr");
    }
    if (const auto it = initial.find("lr"); it != initial.end())
      fixture.lr = AsU32(it->second, "fixture.initial.lr");
    if (fixture.lr != kSentinelReturnPc)
      Fail("fixture.initial.lr must be the fixed native-oracle sentinel");
    if (const auto it = initial.find("ctr"); it != initial.end())
      fixture.ctr = AsU32(it->second, "fixture.initial.ctr");
    if (const auto it = initial.find("cr"); it != initial.end())
      fixture.cr = AsU32(it->second, "fixture.initial.cr");
    if (const auto it = initial.find("xer"); it != initial.end())
      fixture.xer = AsU32(it->second, "fixture.initial.xer");

    if (const auto it = initial.find("memory"); it != initial.end())
    {
      for (const auto& patch_value : AsArray(it->second, "fixture.initial.memory"))
      {
        const auto& patch_object = AsObject(patch_value, "memory patch");
        MemoryPatch patch;
        patch.address = AsU32(Required(patch_object, "address"), "memory patch address");
        patch.bytes =
            DecodeHex(AsString(Required(patch_object, "data_hex"), "memory patch data_hex"),
                      "memory patch data_hex", kMaximumPatchBytes);
        fixture.memory.emplace_back(std::move(patch));
      }
    }

    const auto& observe = AsObject(Required(fixture_object, "observe"), "fixture.observe");
    const JsonArray& observed_gpr =
        AsArray(Required(observe, "gpr"), "fixture.observe.gpr");
    if (observed_gpr.size() > 32)
      Fail("fixture.observe.gpr exceeds its limit");
    for (const auto& register_value : observed_gpr)
      fixture.observe_gpr.push_back(
          static_cast<unsigned>(AsUnsigned(register_value, "fixture.observe.gpr", 31)));
    const JsonArray& observed_memory =
        AsArray(Required(observe, "memory"), "fixture.observe.memory");
    if (observed_memory.size() > kMaximumObservationsPerFixture)
      Fail("fixture.observe.memory exceeds its count limit");
    std::size_t fixture_observed_bytes = 0;
    for (const auto& observation_value : observed_memory)
    {
      const auto& observation_object = AsObject(observation_value, "memory observation");
      MemoryObservation observation;
      observation.address =
          AsU32(Required(observation_object, "address"), "memory observation address");
      observation.size = static_cast<std::size_t>(AsUnsigned(
          Required(observation_object, "size"), "memory observation size", kMaximumObserveBytes));
      if (observation.size > kMaximumObservedBytesPerFixture - fixture_observed_bytes)
        Fail("fixture.observe.memory exceeds its byte limit");
      fixture_observed_bytes += observation.size;
      fixture.observe_memory.emplace_back(observation);
    }
    if (fixture_observed_bytes > kMaximumObservedBytesPerRequest - request_observed_bytes)
      Fail("fixture observations exceed the request byte limit");
    request_observed_bytes += fixture_observed_bytes;
    request.fixtures.emplace_back(std::move(fixture));
  }
  if (request.fixtures.empty())
    Fail("fixtures must not be empty");
  return request;
}

class CpuScope final
{
public:
  CpuScope()
  {
    if (!cpu_init(&m_cpu))
      Fail("DolRecomp cpu_init failed");
  }

  ~CpuScope() { cpu_free(&m_cpu); }
  CPUState& Get() { return m_cpu; }

private:
  CPUState m_cpu{};
};

struct ExecutionGuard
{
  bool alert = false;
};

void MarkAlert(CPUState* cpu)
{
  auto* guard = static_cast<ExecutionGuard*>(cpu->external_user_data);
  if (guard != nullptr)
    guard->alert = true;
  cpu->exception |= PPC_EXC_MACHINE_CHECK;
}

extern "C" std::uint64_t FailClosedRead(CPUState* cpu, std::uint32_t, std::uint8_t)
{
  MarkAlert(cpu);
  return 0;
}

extern "C" void FailClosedWrite(CPUState* cpu, std::uint32_t, std::uint64_t, std::uint8_t)
{
  MarkAlert(cpu);
}

extern "C" std::uint32_t FailClosedRead32(CPUState* cpu, std::uint32_t, std::uint8_t)
{
  MarkAlert(cpu);
  return 0;
}

extern "C" void FailClosedWrite32(CPUState* cpu, std::uint32_t, std::uint32_t, std::uint8_t)
{
  MarkAlert(cpu);
}

extern "C" void FailClosedInstruction(CPUState* cpu, std::uint32_t, std::uint32_t)
{
  MarkAlert(cpu);
}

extern "C" bool FailClosedHostCall(CPUState* cpu, std::uint32_t)
{
  MarkAlert(cpu);
  return false;
}

std::uint8_t* CheckedRam(CPUState& cpu, std::uint32_t address, std::size_t size,
                         const std::string& field)
{
  const std::uint64_t begin = address;
  const std::uint64_t end = begin + size;
  const std::uint64_t ram_end = static_cast<std::uint64_t>(kMem1Base) + cpu.ram_size;
  if (begin < kMem1Base || end < begin || end > ram_end)
    Fail(field + " is outside MEM1");
  return cpu.ram + (address - kMem1Base);
}

picojson::value RunFixture(CPUState& cpu, const Request& request, const Fixture& fixture)
{
  cpu_reset(&cpu);
  ExecutionGuard guard;
  cpu.external_read = FailClosedRead;
  cpu.external_write = FailClosedWrite;
  cpu.external_read32 = FailClosedRead32;
  cpu.external_write32 = FailClosedWrite32;
  cpu.instruction_fallback = FailClosedInstruction;
  cpu.host_call = FailClosedHostCall;
  cpu.external_user_data = &guard;

  auto* const code_sandbox =
      CheckedRam(cpu, request.function->entry_pc, kCodeSandboxBytes, "function code sandbox");
  const std::uint64_t sandbox_end =
      static_cast<std::uint64_t>(request.function->entry_pc) + kCodeSandboxBytes;
  for (const MemoryPatch& patch : fixture.memory)
  {
    const std::uint64_t patch_end = static_cast<std::uint64_t>(patch.address) + patch.bytes.size();
    if (patch.address < sandbox_end && patch_end > request.function->entry_pc)
      Fail("memory patch overlaps function code sandbox");
    std::copy(patch.bytes.begin(), patch.bytes.end(),
              CheckedRam(cpu, patch.address, patch.bytes.size(), "memory patch"));
  }
  std::copy(request.code.begin(), request.code.end(), code_sandbox);
  const std::size_t ram_size = cpu.ram_size;
  const std::vector<std::uint8_t> initial_ram(cpu.ram, cpu.ram + ram_size);

  std::copy(fixture.gpr.begin(), fixture.gpr.end(), cpu.gpr);
  cpu.pc = request.function->entry_pc;
  cpu.lr = fixture.lr;
  cpu.ctr = fixture.ctr;
  cpu.cr = fixture.cr;
  cpu.xer = fixture.xer;
  cpu.msr = 0x00002032u;

  std::string status = "step_limit";
  std::uint64_t dispatches = 0;
  const std::uint64_t function_end =
      static_cast<std::uint64_t>(request.function->entry_pc) + request.function->size;
  while (dispatches < request.max_dispatches)
  {
    if (cpu.pc == fixture.lr)
    {
      status = "returned";
      break;
    }
    if (cpu.pc < request.function->entry_pc || cpu.pc >= function_end || (cpu.pc & 3u) != 0)
    {
      status = "pc_out_of_range";
      break;
    }
    if (cpu.pc < request.function->chunk_begin || cpu.pc >= request.function->chunk_end)
      Fail("pinned function is outside its generated chunk");
    request.function->chunk(&cpu);
    ++dispatches;
    if (guard.alert)
    {
      status = "alert";
      break;
    }
    if (cpu.exception != 0)
    {
      status = "exception";
      break;
    }
  }
  if (status == "step_limit" && cpu.pc == fixture.lr && cpu.exception == 0 && !guard.alert)
    status = "returned";

  const std::size_t sandbox_begin = request.function->entry_pc - kMem1Base;
  const std::size_t sandbox_end_offset = sandbox_begin + kCodeSandboxBytes;
  if (!std::equal(initial_ram.begin() + sandbox_begin, initial_ram.begin() + sandbox_end_offset,
                  code_sandbox))
    status = "code_modified";

  JsonObject final;
  JsonObject registers;
  for (const unsigned index : fixture.observe_gpr)
    registers.emplace(std::to_string(index), picojson::value(Hex32(cpu.gpr[index])));
  final.emplace("gpr", picojson::value(std::move(registers)));
  final.emplace("pc", picojson::value(Hex32(cpu.pc)));
  final.emplace("lr", picojson::value(Hex32(cpu.lr)));
  final.emplace("ctr", picojson::value(Hex32(cpu.ctr)));
  final.emplace("cr", picojson::value(Hex32(cpu.cr)));
  final.emplace("xer", picojson::value(Hex32(cpu.xer)));

  JsonArray observed_memory;
  for (const MemoryObservation& observation : fixture.observe_memory)
  {
    const auto* data = CheckedRam(cpu, observation.address, observation.size, "memory observation");
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
    ram_digest ^= cpu.ram[offset];
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
    if (cpu.ram[offset] == initial_ram[offset])
    {
      ++offset;
      continue;
    }

    const std::size_t span_begin = offset;
    while (offset < ram_size && !(offset >= sandbox_begin && offset < sandbox_end_offset) &&
           cpu.ram[offset] != initial_ram[offset])
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
      change.emplace("after_hex", picojson::value(EncodeHex(cpu.ram + span_begin, span_size)));
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
  // DolRecomp executes a whole generated chunk per call. This field therefore
  // reports bounded native dispatches, not interpreter instruction steps.
  result.emplace("instructions", picojson::value(static_cast<double>(dispatches)));
  result.emplace("final", picojson::value(std::move(final)));
  result.emplace("ram_digest", picojson::value(Hex64(ram_digest)));
  result.emplace("ram_changed_bytes", picojson::value(static_cast<double>(changed_bytes)));
  result.emplace("ram_changes_truncated", picojson::value(changes_truncated));
  result.emplace("ram_changes", picojson::value(std::move(ram_changes)));
  return picojson::value(std::move(result));
}

picojson::value RunRequest(const Request& request)
{
  CpuScope scope;
  JsonArray results;
  for (const Fixture& fixture : request.fixtures)
    results.emplace_back(RunFixture(scope.Get(), request, fixture));

  JsonObject root;
  root.emplace("schema_version", picojson::value(static_cast<double>(kProtocol)));
  root.emplace("engine", picojson::value("moderngekko-dolrecomp-native-original"));
  root.emplace("code_sandbox_bytes", picojson::value(static_cast<double>(kCodeSandboxBytes)));
  JsonObject provenance;
  provenance.emplace("ModernGekko", picojson::value(ORACLE_MODERNGEKKO_COMMIT));
  provenance.emplace("RecompCore", picojson::value(ORACLE_RECOMPCORE_COMMIT));
  provenance.emplace("DolRecomp", picojson::value(ORACLE_DOLRECOMP_COMMIT));
  root.emplace("provenance", picojson::value(std::move(provenance)));
  root.emplace("generated_tree_sha256", picojson::value(std::string(kGeneratedTreeSha256)));
  root.emplace("function", picojson::value(std::string(request.function->name)));
  root.emplace("results", picojson::value(std::move(results)));
  return picojson::value(std::move(root));
}

std::string ReadFile(const std::filesystem::path& path)
{
  std::error_code error;
  const std::uintmax_t size = std::filesystem::file_size(path, error);
  if (error)
    Fail("cannot inspect request file");
  if (size > kMaximumRequestBytes)
    Fail("request file exceeds its limit");
  std::ifstream input(path, std::ios::binary);
  if (!input)
    Fail("cannot open request file");
  return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

picojson::value Identity()
{
  JsonObject root;
  root.emplace("schema_version", picojson::value(static_cast<double>(kProtocol)));
  root.emplace("engine", picojson::value("moderngekko-dolrecomp-native-original"));
  root.emplace("generated_tree_sha256", picojson::value(std::string(kGeneratedTreeSha256)));
  JsonObject provenance;
  provenance.emplace("ModernGekko", picojson::value(ORACLE_MODERNGEKKO_COMMIT));
  provenance.emplace("RecompCore", picojson::value(ORACLE_RECOMPCORE_COMMIT));
  provenance.emplace("DolRecomp", picojson::value(ORACLE_DOLRECOMP_COMMIT));
  root.emplace("provenance", picojson::value(std::move(provenance)));
  return picojson::value(std::move(root));
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
    std::filesystem::path request_path;
    std::filesystem::path result_path;
    bool identity = false;
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
      else if (argument == "--identity")
        identity = true;
      else
        Fail("unknown argument: " + argument);
    }
    if (identity)
    {
      if (!request_path.empty() || !result_path.empty())
        Fail("--identity cannot be combined with request execution");
      std::cout << Identity().serialize() << '\n';
      return 0;
    }
    if (request_path.empty() || result_path.empty())
      Fail("usage: moderngekko-native-oracle --request-file FILE --result-file FILE");

    picojson::value root;
    const std::string error = picojson::parse(root, ReadFile(request_path));
    if (!error.empty())
      Fail("invalid request JSON: " + error);
    WriteFile(result_path, RunRequest(ParseRequest(root)).serialize());
    return 0;
  }
  catch (const std::exception& error)
  {
    std::cerr << "moderngekko-native-oracle: " << error.what() << '\n';
    return 2;
  }
}
