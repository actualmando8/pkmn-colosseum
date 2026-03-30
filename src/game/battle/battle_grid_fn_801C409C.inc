    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_801C40E0
    lis r3, fn_801C4814@ha
    lfs f1, lbl_8047DFB0(r2)
    addi r4, r3, fn_801C4814@l
    lfs f2, lbl_8047DFB4(r2)
    li r3, 0x9
    li r5, 0x0
    bl fn_801C4164
    li r3, 0x54
    bl fn_80166A28
    @L_801C40E0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
