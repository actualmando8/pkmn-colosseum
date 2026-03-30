    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801A8410
    cmplwi r31, 0x0
    beq @L_801A8410
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801A83F8
    lwz r3, 0x14(r30)
    bl fn_801C25E4
@L_801A83F8:
    lwz r3, 0x4(r31)
    bl fn_801C2670
    stw r3, 0x14(r30)
    lwz r3, 0x8(r30)
    lwz r4, 0x8(r31)
    bl fn_801BEEDC
@L_801A8410:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
