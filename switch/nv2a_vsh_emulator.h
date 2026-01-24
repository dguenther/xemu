/*
 * Stub NV2A vertex shader emulator interface for Switch builds.
 *
 * The real implementation is provided by the nv2a_vsh_cpu subproject.
 * This stub allows the Switch port to compile while the dependency
 * is not yet wired into the standalone build.
 */

#ifndef NV2A_VSH_EMULATOR_H
#define NV2A_VSH_EMULATOR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Nv2aVshProgram {
    int dummy;
} Nv2aVshProgram;

typedef struct Nv2aVshCPUXVSSExecutionState {
    uint32_t input_regs[4];
} Nv2aVshCPUXVSSExecutionState;

typedef struct Nv2aVshExecutionState {
    void *opaque;
} Nv2aVshExecutionState;

typedef int Nv2aVshParseResult;

#define NV2AVPR_SUCCESS 0

static inline Nv2aVshParseResult nv2a_vsh_parse_program(
    Nv2aVshProgram *program, uint32_t *data, int length)
{
    (void)program;
    (void)data;
    (void)length;
    return NV2AVPR_SUCCESS;
}

static inline Nv2aVshExecutionState nv2a_vsh_emu_initialize_xss_execution_state(
    Nv2aVshCPUXVSSExecutionState *linkage, float *constants)
{
    (void)linkage;
    (void)constants;
    Nv2aVshExecutionState state = { 0 };
    return state;
}

static inline void nv2a_vsh_emu_execute_track_context_writes(
    Nv2aVshExecutionState *state, Nv2aVshProgram *program, bool *dirty)
{
    (void)state;
    (void)program;
    (void)dirty;
}

static inline void nv2a_vsh_program_destroy(Nv2aVshProgram *program)
{
    (void)program;
}

#endif
