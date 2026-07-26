/**
 * @file sdk_range_800AE9FC.c
 * @brief Candidate Dolphin SDK range, 0x800AE9FC - 0x800B1788.
 */

#include "src/dolphin/sdk_range_800AE3F0.c"

void __DSP_exec_task(DSPTaskInfo* current, DSPTaskInfo* next)
{
    if (current != NULL) {
        DSPSendMailToDSP((u32) current->dram_mmem_addr);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(current->dram_length);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(current->dram_addr);
        while (fn_800AE794() != 0) {
        }
    } else {
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
    }

    DSPSendMailToDSP((u32) next->iram_mmem_addr);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(next->iram_length);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(next->iram_addr);
    while (fn_800AE794() != 0) {
    }

    if (next->state == 0) {
        DSPSendMailToDSP(next->dsp_init_vector);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(0);
        while (fn_800AE794() != 0) {
        }
    } else {
        DSPSendMailToDSP(next->dsp_resume_vector);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP((u32) next->dram_mmem_addr);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(next->dram_length);
        while (fn_800AE794() != 0) {
        }
        DSPSendMailToDSP(next->dram_addr);
        while (fn_800AE794() != 0) {
        }
    }
}

void __DSP_boot_task(DSPTaskInfo* task)
{
    volatile u32 mail;

    while (fn_800AE7A4() == 0) {
    }
    mail = DSPReadMailFromDSP();

    DSPSendMailToDSP(0x80F3A001);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP((u32) task->iram_mmem_addr);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(0x80F3C002);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(task->iram_addr & 0xFFFF);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(0x80F3A002);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(task->iram_length);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(0x80F3B002);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(0);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(0x80F3D001);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(task->dsp_init_vector);
    while (fn_800AE794() != 0) {
    }
}
