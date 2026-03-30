    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    mr r31, r4
    stw r30, 0x10(r1)
    mr. r30, r3
    bne @L_801AFFBC
    b @L_801AFFC4
    @L_801AFF90:
    cmplwi r30, 0x0
    beq @L_801AFFB8
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_801AFFB8
    rlwinm. r0, r31, 0, 24, 24
    beq @L_801AFFB8
    fmr f1, f31
    lwz r3, 0x18(r30)
    bl fn_801C29C4
    @L_801AFFB8:
    lwz r30, 0x0(r30)
    @L_801AFFBC:
    cmplwi r30, 0x0
    bne @L_801AFF90
    @L_801AFFC4:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
