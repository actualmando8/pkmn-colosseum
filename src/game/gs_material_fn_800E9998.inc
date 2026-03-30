    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r7, lbl_80401490@ha
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r26, r3
    addi r31, r7, lbl_80401490@l
    mr r27, r4
    mr r28, r5
    mr r29, r6
    li r30, 0x0
    @L_800E99C4:
    lwz r0, 0x0(r31)
    cmplw r0, r26
    bne @L_800E9B04
    lwz r0, 0x4(r31)
    cmplw r0, r28
    bne @L_800E9B04
    cmplwi r29, 0x0
    beq @L_800E9A84
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_80190E34
    lfs f1, 0x8(r1)
    lfs f0, 0xc(r1)
    fcmpo cr0, f0, f1
    ble @L_800E9A04
    fmr f1, f0
    @L_800E9A04:
    lfs f0, 0x10(r1)
    fcmpo cr0, f0, f1
    ble @L_800E9A14
    fmr f1, f0
    @L_800E9A14:
    lwz r3, 0xc(r29)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    lwz r5, 0x48(r31)
    cmplw r3, r5
    bge @L_800E9A6C
    lis r4, 0xaaab
    lwz r0, 0x4c(r31)
    subi r4, r4, 0x5555
    mulhwu r0, r4, r0
    srwi r0, r0, 1
    cmplw r3, r0
    blt @L_800E9B04
    @L_800E9A6C:
    lwz r0, 0x4c(r31)
    cmplw r3, r0
    ble @L_800E9A84
    mulli r0, r5, 0x3
    cmplw r3, r0
    bgt @L_800E9B04
    @L_800E9A84:
    lfs f1, lbl_8047AB88(r13)
    lfs f0, lbl_8047CBC8(r2)
    fcmpo cr0, f1, f0
    ble @L_800E9AFC
    mr r3, r27
    addi r4, r1, 0x20
    li r24, 0x1
    bl fn_800E3D14
    li r25, 0x0
    b @L_800E9AEC
    @L_800E9AAC:
    slwi r3, r25, 2
    addi r0, r3, 0x8
    lwzx r3, r31, r0
    cmplwi r3, 0x0
    beq @L_800E9AE8
    addi r4, r1, 0x14
    bl fn_800E3D14
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    bl fn_800E0020
    lfs f0, lbl_8047AB88(r13)
    fcmpo cr0, f1, f0
    ble @L_800E9AE8
    li r24, 0x0
    li r25, 0x10
    @L_800E9AE8:
    addi r25, r25, 0x1
    @L_800E9AEC:
    cmplwi r25, 0x10
    blt @L_800E9AAC
    clrlwi. r0, r24, 24
    beq @L_800E9B04
    @L_800E9AFC:
    mr r3, r31
    b @L_800E9B18
    @L_800E9B04:
    addi r30, r30, 0x1
    addi r31, r31, 0x58
    cmplwi r30, 0x6
    blt @L_800E99C4
    li r3, 0x0
    @L_800E9B18:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
