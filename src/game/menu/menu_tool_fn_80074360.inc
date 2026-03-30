    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_8007438C
    li r3, 0x1
    b @L_800745A0
@L_8007438C:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F484
    cmpwi r3, 0x0
    beq @L_800743A8
    li r3, 0x2
    b @L_800745A0
@L_800743A8:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r25, r0, 0x64
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r29, 3, 0, 28
    mr r28, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r27, r29, 2, 0, 29
    add r31, r0, r6
    addi r26, r4, lbl_803B6E08@l
    addi r30, r31, 0x4
@L_800743F0:
    bl OSGetTick
    subf r0, r28, r3
    cmplw r0, r25
    ble @L_80074408
    li r3, 0x3
    b @L_800745A0
@L_80074408:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074424
    li r3, 0x4
    b @L_800745A0
@L_80074424:
    lbz r0, 0x8(r1)
    cmplwi r0, 0x18
    beq @L_80074460
    lwz r12, 0x0(r31)
    cmplwi r12, 0x0
    beq @L_8007444C
    mr r3, r29
    lwz r4, 0x0(r30)
    mtctr r12
    bctrl
@L_8007444C:
    lwzx r0, r26, r27
    cmpwi r0, 0x0
    beq @L_800743F0
    li r3, 0x3e8
    b @L_800745A0
@L_80074460:
    mr r3, r29
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80074480
    li r3, 0x5
    b @L_800745A0
@L_80074480:
    lwz r3, 0xc(r1)
    subis r0, r3, 0x4158
    cmplwi r0, 0x5645
    beq @L_80074498
    li r3, 0x6
    b @L_800745A0
@L_80074498:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_800744C0
    lbz r0, 0x8(r1)
    cmplwi r0, 0x10
    beq @L_800744C0
    li r3, 0x7
    b @L_800745A0
@L_800744C0:
    mr r3, r29
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_800744E0
    li r3, 0x8
    b @L_800745A0
@L_800744E0:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r25, r0, 0x64
    bl OSGetTick
    lis r4, lbl_803B6E08@ha
    mr r26, r3
    rlwinm r27, r29, 2, 0, 29
    addi r28, r4, lbl_803B6E08@l
@L_80074514:
    bl OSGetTick
    subf r0, r26, r3
    cmplw r0, r25
    ble @L_8007452C
    li r3, 0x9
    b @L_800745A0
@L_8007452C:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074548
    li r3, 0xa
    b @L_800745A0
@L_80074548:
    lbz r3, 0x8(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x10
    beq @L_80074560
    li r3, 0xb
    b @L_800745A0
@L_80074560:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8007459C
    lwz r12, 0x0(r31)
    cmplwi r12, 0x0
    beq @L_80074588
    mr r3, r29
    lwz r4, 0x0(r30)
    mtctr r12
    bctrl
@L_80074588:
    lwzx r0, r28, r27
    cmpwi r0, 0x0
    beq @L_80074514
    li r3, 0x3e8
    b @L_800745A0
@L_8007459C:
    li r3, 0x0
@L_800745A0:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
