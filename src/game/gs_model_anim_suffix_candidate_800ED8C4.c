/** Candidate GSmodel animation range, 0x800ED8C4 - 0x800EE044. */
#define GS_MODEL_ANIM_SUFFIX_SPLIT
#define GS_MODEL_ANIM_SUFFIX_MIDDLE
#include "src/game/gs_model_anim_suffix_800ED4D4.c"

extern s32 fn_800D3088(void);
extern f32 fn_800CE318(f64);
extern u8 lbl_80478AF8;
extern const f64 lbl_8047CC80;
extern const f64 lbl_8047CC88;

f32 fn_800ED8C4(u32 type, u32 fractional_frames, f32 frame,
                f32 requested_frame, f32 end_frame, f32 rate)
{
    f32 current = frame;
    f32 result;

    if (lbl_80478AF8 == 1 && fn_800D37CC() == 0x3C) {
        if (fractional_frames == 0) {
            f64 integral = (f64)(s32)frame;
            f32 fraction =
                frame - (f32)(integral - lbl_8047CC80);
            if (fraction > lbl_8047CC78) {
                f64 frames = (f64)(fn_800D3088() - 1);
                f32 advance =
                    (f32)((frames - lbl_8047CC88) * rate);
                if (advance < lbl_8047CC60) {
                    advance = lbl_8047CC5C;
                }
                current = frame + advance;
            }
        }
    }
    if (end_frame > lbl_8047CC5C &&
        current >= end_frame - lbl_8047CC78) {
        if (type == 0) {
            current = end_frame;
        } else if (type == 1) {
            current = fn_800CE318((f64)end_frame);
        }
    }
    if (current != requested_frame) {
        if (current < requested_frame) {
            if (type == 1) {
                result = current + (end_frame - requested_frame);
            } else {
                result = lbl_8047CC5C;
            }
        } else {
            result = current - requested_frame;
        }
    } else {
        result = current - requested_frame;
    }
    if (result < lbl_8047CC5C) {
        result = lbl_8047CC5C;
    }
    return result;
}
