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
    beq @L_801A35E4
    clrlwi. r0, r31, 31
    beq @L_801A35B0
    fmr f1, f31
    lwz r3, 0x7c(r30)
    bl fn_801C29C4
@L_801A35B0:
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A35D4
    fmr f1, f31
    lwz r3, 0x18(r30)
    mr r4, r31
    bl fn_801995D4
@L_801A35D4:
    fmr f1, f31
    lwz r3, 0x80(r30)
    mr r4, r31
    bl fn_801AFF64
@L_801A35E4:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
