    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r26, 0x18(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_8013DB44
    lwz r5, 0x10(r30)
    cmplwi r5, 0x0
    beq @L_8013DB44
    lwz r3, 0x8(r5)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_8013DA10
    li r0, 0x0
    stw r0, 0x14(r30)
    b @L_8013DA10
    @L_8013D9D0:
    lwz r0, 0x14(r30)
    subf r0, r3, r0
    stw r0, 0x14(r30)
    lwz r5, 0x10(r5)
    cmplwi r5, 0x0
    stw r5, 0x10(r30)
    bne @L_8013D9F4
    li r3, 0x0
    b @L_8013DB48
    @L_8013D9F4:
    lwz r3, 0x8(r5)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_8013DA10
    li r0, 0x0
    stw r0, 0x14(r30)
    b @L_8013DA20
    @L_8013DA10:
    lwz r0, 0x14(r30)
    lwz r3, 0x8(r5)
    cmplw r0, r3
    bge @L_8013D9D0
    @L_8013DA20:
    lwz r0, 0x14(r30)
    lis r4, 0x4330
    lwz r3, 0x8(r5)
    stw r0, 0xc(r1)
    lwz r0, lbl_8047AEE0(r13)
    stw r4, 0x8(r1)
    lfd f3, lbl_8047D260(r2)
    cmplwi r0, 0x0
    lfd f0, 0x8(r1)
    stw r3, 0x14(r1)
    fsubs f2, f0, f3
    lfs f4, 0x0(r5)
    stw r4, 0x10(r1)
    lfs f0, 0x4(r5)
    lfd f1, 0x10(r1)
    fsubs f0, f0, f4
    lwz r28, 0x0(r30)
    fsubs f1, f1, f3
    fdivs f1, f2, f1
    fmadds f31, f1, f0, f4
    beq @L_8013DAF8
    cmplwi r28, 0x0
    beq @L_8013DAF8
    bl fn_800E3B3C
    mr. r29, r3
    beq @L_8013DAF8
    li r3, 0x2
    bl fn_800D4604
    bl fn_800D2248
    li r3, 0x1
    bl fn_800D377C
    lwz r3, lbl_8047AEE0(r13)
    li r4, 0x0
    bl fn_800D3410
    li r27, 0x0
    b @L_8013DAE4
    @L_8013DAB0:
    mr r3, r27
    bl fn_800E3B08
    mr. r26, r3
    beq @L_8013DAE0
    cmplw r26, r28
    beq @L_8013DAE0
    bl fn_800E3C64
    clrlwi. r0, r3, 24
    beq @L_8013DAE0
    mr r3, r26
    li r4, 0x3010
    bl fn_800E3760
    @L_8013DAE0:
    addi r27, r27, 0x1
    @L_8013DAE4:
    cmpw r27, r29
    blt @L_8013DAB0
    bl fn_800D3190
    li r3, 0x1
    bl fn_800D4604
    @L_8013DAF8:
    lwz r3, 0x0(r30)
    bl fn_800E5FAC
    clrlwi. r0, r3, 24
    bne @L_8013DB24
    fmr f1, f31
    lwz r3, 0x0(r30)
    lwz r4, 0x4(r30)
    lwz r5, 0x8(r30)
    lwz r6, lbl_8047AEE0(r13)
    bl fn_800E61BC
    b @L_8013DB30
    @L_8013DB24:
    fmr f1, f31
    lwz r3, 0x0(r30)
    bl fn_800E60F0
    @L_8013DB30:
    lwz r0, 0x14(r30)
    li r3, 0x1
    add r0, r0, r31
    stw r0, 0x14(r30)
    b @L_8013DB48
    @L_8013DB44:
    li r3, 0x0
    @L_8013DB48:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r26, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
