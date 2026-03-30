    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r30, r3
    mr r28, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    cmplwi r30, 0x0
    rlwinm r31, r3, 0, 16, 31
    bne @L_8022EE2C
    li r3, 0x0
    b @L_8022F2E4
@L_8022EE2C:
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EE44
    li r3, 0x0
    b @L_8022F2E4
@L_8022EE44:
    mr r3, r30
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EE5C
    li r3, 0x0
    b @L_8022F2E4
@L_8022EE5C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_801F54A4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EE80
    li r3, 0x0
    b @L_8022F2E4
@L_8022EE80:
    mr r3, r30
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EE9C
    li r3, 0x0
    b @L_8022F2E4
@L_8022EE9C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r28, 0, 24, 31
    bne @L_8022F064
    mr r3, r30
    li r4, 0x3e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EED8
    li r3, 0x0
    b @L_8022F2E4
@L_8022EED8:
    mr r3, r30
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EEF8
    li r3, 0x0
    b @L_8022F2E4
@L_8022EEF8:
    mr r3, r30
    bl fn_80205B8C
    bl fn_8011F6D8
    mr r28, r3
    mr r3, r30
    bl fn_80205184
    mr r29, r3
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r3, r5, r4
    rlwinm r0, r28, 0, 24, 31
    mullw r3, r3, r4
    subf r3, r3, r5
    cmpw r3, r0
    bge @L_8022F05C
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x164
    bne @L_8022F05C
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r28, r3
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x3e
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022EFB0
    mr r3, r30
    li r4, 0x3e
    li r5, 0x0
    bl fn_8020248C
@L_8022EFB0:
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EFD8
    mr r3, r30
    li r4, 0xc8
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022EFD8:
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r30
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
    li r3, 0x771b
    bl fn_802624CC
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_8026532C
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    b @L_8022F2E0
@L_8022F05C:
    li r3, 0x0
    b @L_8022F2E4
@L_8022F064:
    cmplwi r0, 0x1
    bne @L_8022F150
    mr r3, r30
    li r4, 0x3e
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022F088
    li r3, 0x0
    b @L_8022F2E4
@L_8022F088:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80208404
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r28, r3
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x2
    li r6, 0x1
    bl fn_80208404
    mr r3, r30
    mr r4, r31
    li r5, 0x1
    bl fn_80265598
    li r3, 0x771c
    bl fn_802624CC
    mr r3, r30
    li r4, 0x0
    li r5, 0x2
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_8026532C
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x2
    li r6, 0x3
    bl fn_80208404
    b @L_8022F2E0
@L_8022F150:
    cmplwi r0, 0x2
    bne @L_8022F2E0
    mr r3, r30
    li r4, 0x3e
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022F174
    li r3, 0x0
    b @L_8022F2E4
@L_8022F174:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r3, 24, 0, 7
    rlwinm r3, r4, 1, 31, 31
    subf r0, r3, r0
    rlwinm r0, r0, 8, 0, 31
    add. r0, r0, r3
    beq @L_8022F19C
    li r3, 0x0
    b @L_8022F2E4
@L_8022F19C:
    mr r3, r30
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, 0x0
    bl fn_802080A8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r28, r3
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x3e
    bl fn_80202810
    mr r3, r30
    li r4, 0x8
    bl fn_80202810
    mr r3, r30
    li r4, 0x17
    bl fn_80202810
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F244
    mr r3, r30
    li r4, 0xc8
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022F244:
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F26C
    mr r3, r30
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022F26C:
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x771e
    li r7, 0x1
    bl fn_802080A8
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, 0x2
    bl fn_802080A8
    bl fn_8026246C
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_8026532C
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, 0x3
    bl fn_802080A8
@L_8022F2E0:
    li r3, 0x1
@L_8022F2E4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
