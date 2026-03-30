    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    mr. r31, r3
    bne @L_801AFF44
    b @L_801AFF4C
    @L_801AFF20:
    cmplwi r31, 0x0
    beq @L_801AFF40
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801AFF40
    fmr f1, f31
    lwz r3, 0x18(r31)
    bl fn_801C29C4
    @L_801AFF40:
    lwz r31, 0x0(r31)
    @L_801AFF44:
    cmplwi r31, 0x0
    bne @L_801AFF20
    @L_801AFF4C:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
