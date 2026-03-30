    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    li r0, 0x0
    stw r31, 0xfc(r1)
    stw r30, 0xf8(r1)
    mr r30, r3
    stw r0, lbl_8047ABA0(r13)
    lwz r4, 0x10(r3)
    lwz r5, 0x14(r3)
    lwz r6, 0xc(r3)
    lfs f1, 0xd4(r3)
    bl fn_800EB6E0
    lwz r31, 0xc(r30)
    cmplwi r31, 0x0
    bne @L_800EB5F0
    la r3, lbl_8047CC40(r2)
    li r4, 0x47c
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EB5F0:
    cmplwi r31, 0x0
    beq @L_800EB638
    bne @L_800EB60C
    la r3, lbl_8047CC40(r2)
    li r4, 0x25d
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EB60C:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EB628
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EB628
    li r3, 0x1
    @L_800EB628:
    cmpwi r3, 0x0
    beq @L_800EB638
    mr r3, r31
    bl fn_8019D9DC
    @L_800EB638:
    addi r31, r31, 0x44
    addi r3, r1, 0xc8
    addi r4, r30, 0x18
    bl fn_800E0560
    lfs f1, 0x24(r30)
    addi r3, r1, 0x98
    bl fn_800E053C
    lfs f1, 0x28(r30)
    addi r3, r1, 0x68
    bl fn_800E0518
    lfs f1, 0x2c(r30)
    addi r3, r1, 0x38
    bl fn_800E04F4
    addi r3, r1, 0x8
    addi r4, r30, 0x30
    bl fn_800E042C
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x8
    bl fn_800E0290
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0xc8
    bl fn_800E0290
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x98
    bl fn_800E0290
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x68
    bl fn_800E0290
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x38
    bl fn_800E0290
    lwz r0, 0x104(r1)
    lwz r31, 0xfc(r1)
    lwz r30, 0xf8(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
