#ifndef GAME_CAMERA_TYPES_H
#define GAME_CAMERA_TYPES_H

#include "game/gs_scene_types.h"

typedef struct CameraPadState {
    /* 0x00 */ u8 mode;
    /* 0x01 */ u8 flags[3];
    /* 0x04 */ GSSceneVec3 direction;
    /* 0x10 */ GSSceneVec3 rotation;
    /* 0x1C */ GSSceneVec3 position;
    /* 0x28 */ GSSceneVec3 view;
    /* 0x34 */ u32 targetGroup;
    /* 0x38 */ u32 targetId;
    /* 0x3C */ s32 targetSubId;
    /* 0x40 */ f32 height;
    /* 0x44 */ f32 distance;
    /* 0x48 */ f32 fov;
    /* 0x4C */ u8 targetMoveActive;
    /* 0x4D */ u8 targetOffsetMoveActive;
    /* 0x4E */ u8 positionMoveActive;
    /* 0x4F */ u8 rotationMoveActive;
    /* 0x50 */ GSSceneVec3 targetMoveEnd;
    /* 0x5C */ GSSceneVec3 targetMoveStart;
    /* 0x68 */ f32 targetMoveDuration;
    /* 0x6C */ f32 targetMoveTime;
    /* 0x70 */ GSSceneVec3 targetOffsetMoveEnd;
    /* 0x7C */ GSSceneVec3 targetOffsetMoveStart;
    /* 0x88 */ f32 targetOffsetMoveDuration;
    /* 0x8C */ f32 targetOffsetMoveTime;
    /* 0x90 */ GSSceneVec3 positionMoveEnd;
    /* 0x9C */ GSSceneVec3 positionMoveStart;
    /* 0xA8 */ f32 positionMoveDuration;
    /* 0xAC */ f32 positionMoveTime;
    /* 0xB0 */ GSSceneVec3 rotationMoveEnd;
    /* 0xBC */ GSSceneVec3 rotationMoveStart;
    /* 0xC8 */ f32 rotationMoveDuration;
    /* 0xCC */ f32 rotationMoveTime;
    /* 0xD0 */ u32 animationGroup;
    /* 0xD4 */ u32 animationId;
    /* 0xD8 */ GSSceneVec3 offsetPosition;
    /* 0xE4 */ GSSceneVec3 offsetRotation;
    /* 0xF0 */ GSSceneVec3 offsetScale;
} CameraPadState; /* size 0xFC; followed by the 0xB0 render-camera snapshot */

#endif /* GAME_CAMERA_TYPES_H */
