    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    li r0, 0x0
    stw r31, 0x1c(r1)
    stw r4, 0x8(r1)
    stw r5, 0xc(r1)
    lwz r3, 0x8(r1)
    stw r30, 0x18(r1)
    lwz r4, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_800F9318
    mr r30, r3
    lwz r0, 0x38(r30)
    mr r31, r30
    cmplwi r0, 0x0
    beq @L_8017F704
    lwz r12, 0x38(r31)
    mtctr r12
    bctrl
    @L_8017F704:
    mr r3, r31
    bl fn_8009EFE4
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
