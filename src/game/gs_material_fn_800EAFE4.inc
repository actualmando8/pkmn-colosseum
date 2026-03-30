    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stmw r24, 0xa0(r1)
    lis r5, lbl_80270EB8@ha
    mr r27, r3
    mr r25, r4
    mr r26, r6
    addi r31, r5, lbl_80270EB8@l
    bl fn_8019F01C
    mr r0, r3
    li r3, 0x0
    mr r24, r0
    li r4, 0x2
    bl fn_801AB63C
    mr r3, r24
    addi r4, r1, 0x68
    bl fn_80197A64
    lwz r29, 0x14(r27)
    mr r27, r3
    lfs f31, lbl_8047CC18(r2)
    mr r30, r26
    li r28, 0x0
    b @L_800EB23C
    @L_800EB04C:
    lwz r26, 0x4(r29)
    cmplwi r26, 0x0
    bne @L_800EB068
    addi r5, r31, 0x0
    la r3, lbl_8047CC10(r2)
    li r4, 0x65
    bl fn_80196E10
    @L_800EB068:
    lfs f0, 0x8(r26)
    fcmpo cr0, f0, f31
    cror eq, gt, eq
    bne @L_800EB0F4
    lwz r24, 0x4(r26)
    cmplwi r24, 0x0
    beq @L_800EB0C4
    bne @L_800EB098
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EB098:
    lwz r4, 0x14(r24)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EB0B4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EB0B4
    li r3, 0x1
    @L_800EB0B4:
    cmpwi r3, 0x0
    beq @L_800EB0C4
    mr r3, r24
    bl fn_8019D9DC
    @L_800EB0C4:
    cmplwi r27, 0x0
    beq @L_800EB0E8
    lwz r3, 0x4(r26)
    addi r5, r1, 0x38
    lwz r4, 0x78(r3)
    addi r3, r3, 0x44
    bl fn_800A2D98
    addi r24, r1, 0x38
    b @L_800EB1FC
    @L_800EB0E8:
    lwz r3, 0x4(r26)
    addi r24, r3, 0x44
    b @L_800EB1FC
    @L_800EB0F4:
    lfs f0, lbl_8047CC1C(r2)
    stfs f0, 0x64(r1)
    stfs f0, 0x60(r1)
    stfs f0, 0x5c(r1)
    stfs f0, 0x58(r1)
    stfs f0, 0x54(r1)
    stfs f0, 0x50(r1)
    stfs f0, 0x4c(r1)
    stfs f0, 0x48(r1)
    stfs f0, 0x44(r1)
    stfs f0, 0x40(r1)
    stfs f0, 0x3c(r1)
    stfs f0, 0x38(r1)
    b @L_800EB1F0
    @L_800EB12C:
    lwz r0, 0x4(r26)
    cmplwi r0, 0x0
    bne @L_800EB148
    addi r5, r31, 0xc
    la r3, lbl_8047CC10(r2)
    li r4, 0x7e
    bl fn_80196E10
    @L_800EB148:
    lwz r24, 0x4(r26)
    cmplwi r24, 0x0
    beq @L_800EB194
    bne @L_800EB168
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EB168:
    lwz r4, 0x14(r24)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EB184
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EB184
    li r3, 0x1
    @L_800EB184:
    cmpwi r3, 0x0
    beq @L_800EB194
    mr r3, r24
    bl fn_8019D9DC
    @L_800EB194:
    addic. r0, r24, 0x44
    bne @L_800EB1AC
    la r3, lbl_8047CC10(r2)
    li r4, 0x81
    la r5, lbl_8047CC20(r2)
    bl fn_80196E10
    @L_800EB1AC:
    lwz r0, 0x78(r24)
    cmplwi r0, 0x0
    bne @L_800EB1C8
    addi r5, r31, 0x1c
    la r3, lbl_8047CC10(r2)
    li r4, 0x82
    bl fn_80196E10
    @L_800EB1C8:
    lwz r4, 0x78(r24)
    addi r3, r24, 0x44
    addi r5, r1, 0x8
    bl fn_800A2D98
    addi r4, r1, 0x38
    lfs f1, 0x8(r26)
    mr r5, r4
    addi r3, r1, 0x8
    bl fn_801A85F0
    lwz r26, 0x0(r26)
    @L_800EB1F0:
    cmplwi r26, 0x0
    bne @L_800EB12C
    addi r24, r1, 0x38
    @L_800EB1FC:
    cmplwi r27, 0x0
    beq @L_800EB214
    mr r3, r24
    mr r4, r27
    addi r5, r1, 0x38
    bl fn_800A2D98
    @L_800EB214:
    mr r3, r25
    mr r4, r24
    addi r5, r1, 0x8
    bl fn_800A2D98
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_800E0628
    lwz r29, 0x0(r29)
    addi r30, r30, 0x30
    addi r28, r28, 0x1
    @L_800EB23C:
    cmpwi r28, 0xa
    bge @L_800EB24C
    cmplwi r29, 0x0
    bne @L_800EB04C
    @L_800EB24C:
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    lmw r24, 0xa0(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
