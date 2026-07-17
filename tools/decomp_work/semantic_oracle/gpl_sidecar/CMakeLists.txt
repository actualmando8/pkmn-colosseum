# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.20)

if(APPLE)
  project(ModernGekkoDolphinOracle LANGUAGES C CXX OBJCXX)
else()
  project(ModernGekkoDolphinOracle LANGUAGES C CXX)
endif()

if(NOT DEFINED MODERNGEKKO_SOURCE_DIR)
  message(FATAL_ERROR "Set MODERNGEKKO_SOURCE_DIR to the pinned ModernGekko checkout")
endif()

get_filename_component(MODERNGEKKO_SOURCE_DIR
  "${MODERNGEKKO_SOURCE_DIR}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(ORACLE_MODERNGEKKO_COMMIT "11237c119a5d8e907a20e9cae1c357df149aaa47")
set(ORACLE_RECOMPCORE_COMMIT "1873066167f3d03b39771b547f280d2b970427b6")
set(ORACLE_DOLRECOMP_COMMIT "a2b02e5a515fc8971cc551ad51c9e26a9815daad")
set(ORACLE_DRIVER "${CMAKE_CURRENT_SOURCE_DIR}/../driver.py")
set(ORACLE_PIN_MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/../moderngekko_pins.json")
set(ORACLE_PRE_BUILD_STATE
  "${CMAKE_CURRENT_BINARY_DIR}/moderngekko-dolphin-oracle.pre-build.json")
find_package(Python3 REQUIRED COMPONENTS Interpreter)

function(require_exact_git_checkout repository expected_commit label)
  execute_process(
    COMMAND git -C "${repository}" rev-parse HEAD
    RESULT_VARIABLE head_result
    OUTPUT_VARIABLE actual_commit
    ERROR_VARIABLE head_error
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT head_result EQUAL 0)
    message(FATAL_ERROR "Cannot inspect ${label} HEAD: ${head_error}")
  endif()
  if(NOT actual_commit STREQUAL expected_commit)
    message(FATAL_ERROR
      "${label} pin mismatch: expected ${expected_commit}, got ${actual_commit}")
  endif()

  execute_process(
    COMMAND git -C "${repository}" status --porcelain=v1 --untracked-files=all
    RESULT_VARIABLE status_result
    OUTPUT_VARIABLE status_output
    ERROR_VARIABLE status_error
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT status_result EQUAL 0)
    message(FATAL_ERROR "Cannot inspect ${label} worktree: ${status_error}")
  endif()
  if(NOT status_output STREQUAL "")
    message(FATAL_ERROR "${label} checkout is dirty: ${status_output}")
  endif()
endfunction()

set(RECOMPCORE_SOURCE_DIR "${MODERNGEKKO_SOURCE_DIR}/vendor/dolphin")
set(DOLRECOMP_SOURCE_DIR "${RECOMPCORE_SOURCE_DIR}/DolRecomp")
require_exact_git_checkout(
  "${MODERNGEKKO_SOURCE_DIR}" "${ORACLE_MODERNGEKKO_COMMIT}" "ModernGekko")
require_exact_git_checkout(
  "${RECOMPCORE_SOURCE_DIR}" "${ORACLE_RECOMPCORE_COMMIT}" "RecompCore")
require_exact_git_checkout(
  "${DOLRECOMP_SOURCE_DIR}" "${ORACLE_DOLRECOMP_COMMIT}" "DolRecomp")

execute_process(
  COMMAND git -C "${MODERNGEKKO_SOURCE_DIR}" rev-parse HEAD:vendor/dolphin
  OUTPUT_VARIABLE RECOMPCORE_GITLINK
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY)
if(NOT RECOMPCORE_GITLINK STREQUAL "${ORACLE_RECOMPCORE_COMMIT}")
  message(FATAL_ERROR "ModernGekko's RecompCore gitlink does not match the pin")
endif()
execute_process(
  COMMAND git -C "${RECOMPCORE_SOURCE_DIR}" rev-parse HEAD:DolRecomp
  OUTPUT_VARIABLE DOLRECOMP_GITLINK
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY)
if(NOT DOLRECOMP_GITLINK STREQUAL "${ORACLE_DOLRECOMP_COMMIT}")
  message(FATAL_ERROR "RecompCore's DolRecomp gitlink does not match the pin")
endif()

set(MODERNGEKKO_ENABLE_DOLPHIN_RUNTIME ON CACHE BOOL "" FORCE)
set(MODERNGEKKO_ENABLE_DOLPHIN_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# ModernGekko owns the pinned RecompCore/Dolphin dependency.  Nothing from that
# GPL dependency is copied into the campaign repository or linked into its
# normal tools; this standalone target is built and executed out of process.
add_subdirectory("${MODERNGEKKO_SOURCE_DIR}" moderngekko EXCLUDE_FROM_ALL)

add_executable(moderngekko-dolphin-oracle
  dolphin_oracle.cpp
  "${MODERNGEKKO_SOURCE_DIR}/vendor/dolphin/Source/UnitTests/StubHost.cpp")

# Run the full manifest/origin/gitlink/recursive-clean verifier at build time,
# not only while configuring.  The finalizer repeats the same snapshot after
# linking and binds it, the local C++/CMake inputs, and the binary hash in an
# adjacent attestation consumed by the Python driver.
add_custom_target(moderngekko-dolphin-oracle-preflight
  COMMAND "${Python3_EXECUTABLE}" "${ORACLE_DRIVER}" attest-build-pre
    --checkout "${MODERNGEKKO_SOURCE_DIR}"
    --pins "${ORACLE_PIN_MANIFEST}"
    --output "${ORACLE_PRE_BUILD_STATE}"
  BYPRODUCTS "${ORACLE_PRE_BUILD_STATE}"
  COMMENT "Verifying pinned semantic-sidecar inputs before build"
  VERBATIM)
add_dependencies(moderngekko-dolphin-oracle
  moderngekko-dolphin-oracle-preflight)

add_custom_command(TARGET moderngekko-dolphin-oracle POST_BUILD
  COMMAND "${Python3_EXECUTABLE}" "${ORACLE_DRIVER}" attest-build-finalize
    --checkout "${MODERNGEKKO_SOURCE_DIR}"
    --pins "${ORACLE_PIN_MANIFEST}"
    --pre-state "${ORACLE_PRE_BUILD_STATE}"
    --binary "$<TARGET_FILE:moderngekko-dolphin-oracle>"
    --output "$<TARGET_FILE:moderngekko-dolphin-oracle>.attestation.json"
  COMMENT "Attesting pinned semantic-sidecar build"
  VERBATIM)

# This target also refreshes/verifies the attestation when the executable is
# already up to date.  Consumers should build this target explicitly.
add_custom_target(moderngekko-dolphin-oracle-attested
  COMMAND "${Python3_EXECUTABLE}" "${ORACLE_DRIVER}" attest-build-finalize
    --checkout "${MODERNGEKKO_SOURCE_DIR}"
    --pins "${ORACLE_PIN_MANIFEST}"
    --pre-state "${ORACLE_PRE_BUILD_STATE}"
    --binary "$<TARGET_FILE:moderngekko-dolphin-oracle>"
    --output "$<TARGET_FILE:moderngekko-dolphin-oracle>.attestation.json"
  DEPENDS moderngekko-dolphin-oracle
  COMMENT "Verifying semantic-sidecar build attestation"
  VERBATIM)

target_compile_features(moderngekko-dolphin-oracle PRIVATE cxx_std_23)
target_include_directories(moderngekko-dolphin-oracle PRIVATE
  "${MODERNGEKKO_SOURCE_DIR}/vendor/dolphin/Source/Core"
  "${MODERNGEKKO_SOURCE_DIR}/vendor/dolphin/GXRuntime/include"
  "${MODERNGEKKO_SOURCE_DIR}/vendor/dolphin/Source/UnitTests"
  "${MODERNGEKKO_SOURCE_DIR}/vendor/picojson")
target_link_libraries(moderngekko-dolphin-oracle PRIVATE core uicommon)
target_compile_definitions(moderngekko-dolphin-oracle PRIVATE
  ORACLE_MODERNGEKKO_COMMIT="${ORACLE_MODERNGEKKO_COMMIT}"
  ORACLE_RECOMPCORE_COMMIT="${ORACLE_RECOMPCORE_COMMIT}"
  ORACLE_DOLRECOMP_COMMIT="${ORACLE_DOLRECOMP_COMMIT}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  target_compile_definitions(moderngekko-dolphin-oracle PRIVATE _ARCH_64=1)
else()
  target_compile_definitions(moderngekko-dolphin-oracle PRIVATE _ARCH_32=1)
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x64|x86_64|amd64|AMD64")
  target_compile_definitions(moderngekko-dolphin-oracle PRIVATE _M_X86_64=1)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
  target_compile_definitions(moderngekko-dolphin-oracle PRIVATE _M_ARM_64=1)
else()
  target_compile_definitions(moderngekko-dolphin-oracle PRIVATE _M_GENERIC=1)
endif()
