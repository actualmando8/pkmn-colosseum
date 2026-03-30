    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801C265C
    cmplwi r31, 0x0
    beq @L_801C2620
    lwz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801C2618
    lwz r3, 0x14(r31)
    bl fn_8019B5E8
    @L_801C2618:
    li r0, 0x0
    stw r0, 0x14(r31)
    @L_801C2620:
    cmplwi r31, 0x0
    beq @L_801C2644
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801C263C
    lwz r3, 0x18(r31)
    bl fn_801A05EC
    @L_801C263C:
    li r0, 0x0
    stw r0, 0x18(r31)
    @L_801C2644:
    cmplwi r31, 0x0
    beq @L_801C265C
    lis r3, lbl_80466DB8@ha
    mr r4, r31
    addi r3, r3, lbl_80466DB8@l
    bl fn_801AA498
    @L_801C265C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
