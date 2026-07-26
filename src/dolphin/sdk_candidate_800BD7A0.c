/** Residual GX/SDK candidate, 0x800BD7A0 - 0x800BE30C. */
#define SDK_800BC618_SUFFIX_ACTIVE
#include "src/dolphin/sdk_range_800BB30C.c"

static inline u32 ReadCPCounter(u32 low, u32 high)
{
    u16 high0;
    u16 low0;
    u16 high1;

    high0 = __cpReg[high];
    do {
        high1 = high0;
        low0 = __cpReg[low];
        high0 = __cpReg[high];
    } while (high0 != high1);
    return (high0 << 16) | low0;
}

void fn_800BE164(u32* count0, u32* count1)
{
    u32 cpCounter0;
    u32 cpCounter1;
    u32 cpCounter2;
    u32 cpCounter3;

    cpCounter0 = ReadCPCounter(32, 33);
    cpCounter1 = ReadCPCounter(34, 35);
    cpCounter2 = ReadCPCounter(36, 37);
    cpCounter3 = ReadCPCounter(38, 39);

    switch (gx->perf0) {
    case GX_PERF0_CLIP_RATIO:
        *count0 = cpCounter1 * 1000 / cpCounter0;
        break;
    case GX_PERF0_VERTICES:
    case GX_PERF0_CLIP_VTX:
    case GX_PERF0_CLIP_CLKS:
    case GX_PERF0_XF_WAIT_IN:
    case GX_PERF0_XF_WAIT_OUT:
    case GX_PERF0_XF_XFRM_CLKS:
    case GX_PERF0_XF_LIT_CLKS:
    case GX_PERF0_XF_BOT_CLKS:
    case GX_PERF0_XF_REGLD_CLKS:
    case GX_PERF0_XF_REGRD_CLKS:
    case GX_PERF0_TRIANGLES:
    case GX_PERF0_TRIANGLES_CULLED:
    case GX_PERF0_TRIANGLES_PASSED:
    case GX_PERF0_TRIANGLES_SCISSORED:
    case GX_PERF0_TRIANGLES_0TEX:
    case GX_PERF0_TRIANGLES_1TEX:
    case GX_PERF0_TRIANGLES_2TEX:
    case GX_PERF0_TRIANGLES_3TEX:
    case GX_PERF0_TRIANGLES_4TEX:
    case GX_PERF0_TRIANGLES_5TEX:
    case GX_PERF0_TRIANGLES_6TEX:
    case GX_PERF0_TRIANGLES_7TEX:
    case GX_PERF0_TRIANGLES_8TEX:
    case GX_PERF0_TRIANGLES_0CLR:
    case GX_PERF0_TRIANGLES_1CLR:
    case GX_PERF0_TRIANGLES_2CLR:
    case GX_PERF0_QUAD_0CVG:
    case GX_PERF0_QUAD_NON0CVG:
    case GX_PERF0_QUAD_1CVG:
    case GX_PERF0_QUAD_2CVG:
    case GX_PERF0_QUAD_3CVG:
    case GX_PERF0_QUAD_4CVG:
    case GX_PERF0_AVG_QUAD_CNT:
    case GX_PERF0_CLOCKS:
        *count0 = cpCounter0;
        break;
    case GX_PERF0_NONE:
    default:
        *count0 = 0;
        break;
    }

    switch (gx->perf1) {
    case GX_PERF1_TEXELS:
        *count1 = cpCounter3 * 4;
        break;
    case GX_PERF1_TC_CHECK1_2:
        *count1 = cpCounter2 + cpCounter3 * 2;
        break;
    case GX_PERF1_TC_CHECK3_4:
        *count1 = cpCounter2 * 3 + cpCounter3 * 4;
        break;
    case GX_PERF1_TC_CHECK5_6:
        *count1 = cpCounter2 * 5 + cpCounter3 * 6;
        break;
    case GX_PERF1_TC_CHECK7_8:
        *count1 = cpCounter2 * 7 + cpCounter3 * 8;
        break;
    case GX_PERF1_TX_IDLE:
    case GX_PERF1_TX_REGS:
    case GX_PERF1_TX_MEMSTALL:
    case GX_PERF1_TC_MISS:
    case GX_PERF1_VC_ELEMQ_FULL:
    case GX_PERF1_VC_MISSQ_FULL:
    case GX_PERF1_VC_MEMREQ_FULL:
    case GX_PERF1_VC_STATUS7:
    case GX_PERF1_VC_MISSREP_FULL:
    case GX_PERF1_VC_STREAMBUF_LOW:
    case GX_PERF1_VC_ALL_STALLS:
    case GX_PERF1_VERTICES:
    case GX_PERF1_CLOCKS:
        *count1 = cpCounter3;
        break;
    case GX_PERF1_FIFO_REQ:
    case GX_PERF1_CALL_REQ:
    case GX_PERF1_VC_MISS_REQ:
    case GX_PERF1_CP_ALL_REQ:
        *count1 = cpCounter2;
        break;
    case GX_PERF1_NONE:
    default:
        *count1 = 0;
        break;
    }
}
