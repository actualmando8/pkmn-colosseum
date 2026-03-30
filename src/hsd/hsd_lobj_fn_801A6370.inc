    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    beq @L_801A63F0
    mr r31, r3
    b @L_801A63E8
    @L_801A6398:
    cmplwi r31, 0x0
    beq @L_801A63E4
    fmr f1, f31
    lwz r3, 0x48(r31)
    bl fn_801C29C4
    cmplwi r31, 0x0
    beq @L_801A63BC
    lwz r3, 0x18(r31)
    b @L_801A63C0
    @L_801A63BC:
    li r3, 0x0
    @L_801A63C0:
    fmr f1, f31
    bl fn_80191E38
    cmplwi r31, 0x0
    beq @L_801A63D8
    lwz r3, 0x1c(r31)
    b @L_801A63DC
    @L_801A63D8:
    li r3, 0x0
    @L_801A63DC:
    fmr f1, f31
    bl fn_80191E38
    @L_801A63E4:
    lwz r31, 0xc(r31)
    @L_801A63E8:
    cmplwi r31, 0x0
    bne @L_801A6398
    @L_801A63F0:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
