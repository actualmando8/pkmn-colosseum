    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r26, 0x18(r1)
    lfd f31, lbl_8047CB58(r2)
    mr r26, r3
    li r28, 0x0
    clrlwi r30, r4, 24
    li r29, 0x0
    lis r31, 0x4330
    b @L_800E3900
    @L_800E38B8:
    lwz r0, lbl_8047AB74(r13)
    add r27, r0, r29
    lwz r0, 0x0(r27)
    clrlwi. r0, r0, 31
    beq @L_800E38F8
    mr r3, r27
    bl fn_800ED6AC
    clrlwi r0, r3, 24
    cmplw r30, r0
    bne @L_800E38F8
    stw r26, 0xc(r1)
    mr r3, r27
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f31
    bl fn_800ED0D0
    @L_800E38F8:
    addi r29, r29, 0x170
    addi r28, r28, 0x1
    @L_800E3900:
    lwz r0, lbl_8047AB78(r13)
    cmplw r28, r0
    blt @L_800E38B8
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r26, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
