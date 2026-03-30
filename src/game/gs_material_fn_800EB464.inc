    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r3, 31
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    beq @L_800EB4A0
    lis r5, 0x5555
    lbz r0, 0x0(r4)
    addi r5, r5, 0x5556
    mulhw r5, r5, r0
    srwi r0, r5, 31
    add r0, r5, r0
    stw r0, lbl_8047AB98(r13)
    @L_800EB4A0:
    rlwinm. r0, r3, 0, 30, 30
    beq @L_800EB508
    lwz r0, 0xc(r30)
    clrlwi. r0, r0, 31
    beq @L_800EB4C8
    lwz r0, lbl_8047AB98(r13)
    lwz r3, 0x8(r30)
    mulli r0, r0, 0x30
    add r31, r3, r0
    b @L_800EB4CC
    @L_800EB4C8:
    lwz r31, 0x4(r30)
    @L_800EB4CC:
    lfs f1, 0x0(r4)
    addi r3, r1, 0x8
    lfs f2, 0x4(r4)
    lfs f3, 0x8(r4)
    bl fn_800E01F4
    addi r3, r1, 0x8
    mr r4, r31
    mr r5, r3
    bl fn_800DFF98
    lwz r3, 0x0(r30)
    bl fn_800E3C5C
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_80191358
    @L_800EB508:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
