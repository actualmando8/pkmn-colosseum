    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x14(r3)
    bl fn_801C25E4
    lwz r3, 0xc(r31)
    li r4, 0x14
    bl fn_80193AF0
    lwz r3, 0x8(r31)
    bl fn_801BBE60
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801A6BD0
    lwz r3, 0x18(r31)
    bl fn_801B42C0
    @L_801A6BD0:
    lwz r0, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_801A6BEC
    lwz r3, 0x1c(r31)
    li r4, 0x7
    li r5, 0x1
    bl fn_801B7178
    @L_801A6BEC:
    lwz r0, 0x10(r31)
    cmplwi r0, 0x0
    beq @L_801A6C04
    lwz r3, 0x10(r31)
    li r4, 0xc
    bl fn_80193AF0
    @L_801A6C04:
    lis r4, lbl_8036CB30@ha
    mr r3, r31
    addi r4, r4, lbl_8036CB30@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
