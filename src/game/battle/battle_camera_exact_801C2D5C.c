/**
 * @file battle_camera_exact_801C2D5C.c
 * @brief Strict battle-camera mode setters, 0x801C2D5C - 0x801C2D80.
 */
#include "dolphin/types.h"

extern u8 lbl_8047B398;
extern u8 lbl_8047B399;

void battleCameraDoFull(void)
{
    lbl_8047B399 = 0;
}

void battleCameraDoSimple(void)
{
    lbl_8047B399 = 1;
}

void battleCameraDisable(void)
{
    lbl_8047B398 = 1;
}
