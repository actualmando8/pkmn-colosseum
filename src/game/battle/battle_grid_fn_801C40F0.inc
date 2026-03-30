    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_801C4120
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lbz r0, 0x1(r3)
    extsb r3, r0
    b @L_801C4150
    @L_801C4120:
    lis r3, lbl_80466E30@ha
    addi r31, r3, lbl_80466E30@l
    b @L_801C4130
    @L_801C412C:
    bl fn_800F0308
    @L_801C4130:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x1
    beq @L_801C412C
    bl fn_800F0308
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lbz r0, 0x1(r3)
    extsb r3, r0
    @L_801C4150:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
