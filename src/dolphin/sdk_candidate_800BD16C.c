/** Residual GX/SDK candidate, 0x800BD16C - 0x800BD394. */
#define SDK_800BC618_SUFFIX_ACTIVE
#include "src/dolphin/sdk_range_800BB30C.c"

typedef f32 GXProjectMtx[3][4];

void GXProject(f32 x, f32 y, f32 z, const GXProjectMtx mtx,
               const f32* projection, const f32* viewport,
               f32* screen_x, f32* screen_y, f32* screen_z)
{
    f32 eye_x;
    f32 eye_y;
    f32 eye_z;
    f32 clip_x;
    f32 clip_y;
    f32 clip_z;
    f32 reciprocal_w;

    eye_x = mtx[0][3] +
            (mtx[0][2] * z + (mtx[0][0] * x + mtx[0][1] * y));
    eye_y = mtx[1][3] +
            (mtx[1][2] * z + (mtx[1][0] * x + mtx[1][1] * y));
    eye_z = mtx[2][3] +
            (mtx[2][2] * z + (mtx[2][0] * x + mtx[2][1] * y));

    if (projection[0] == 0.0F) {
        clip_x = eye_x * projection[1] + eye_z * projection[2];
        clip_y = eye_y * projection[3] + eye_z * projection[4];
        clip_z = projection[6] + eye_z * projection[5];
        reciprocal_w = 1.0F / -eye_z;
    } else {
        clip_x = projection[2] + eye_x * projection[1];
        clip_y = projection[4] + eye_y * projection[3];
        clip_z = projection[6] + eye_z * projection[5];
        reciprocal_w = 1.0F;
    }

    *screen_x = viewport[2] / 2.0F +
                (viewport[0] +
                 reciprocal_w * (clip_x * viewport[2] / 2.0F));
    *screen_y = viewport[3] / 2.0F +
                (viewport[1] +
                 reciprocal_w * (-clip_y * viewport[3] / 2.0F));
    *screen_z = viewport[5] +
                reciprocal_w * (clip_z * (viewport[5] - viewport[4]));
}
