/**
 * Code-exact synthVolume at 0x8014C984.
 *
 * This stays a candidate because compiling it alone duplicates and reorders
 * constants shared with ZeroOffsetHandler and the residual synth functions.
 */
#define SYNTH_SUFFIX_SPLIT
#define SYNTH_SUFFIX_VOLUME
#include "src/musyx/runtime/synth_suffix_8014B044.c"
