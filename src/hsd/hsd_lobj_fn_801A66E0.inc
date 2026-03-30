    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_801A6774
    mr r30, r3
    b @L_801A676C
    @L_801A6704:
    cmplwi r30, 0x0
    beq @L_801A6768
    mr r31, r30
    b @L_801A6760
    @L_801A6714:
    cmplwi r31, 0x0
    beq @L_801A675C
    lwz r3, 0x48(r31)
    bl fn_801C25E4
    li r0, 0x0
    cmplwi r31, 0x0
    stw r0, 0x48(r31)
    beq @L_801A673C
    lwz r3, 0x18(r31)
    b @L_801A6740
    @L_801A673C:
    li r3, 0x0
    @L_801A6740:
    bl fn_80191E88
    cmplwi r31, 0x0
    beq @L_801A6754
    lwz r3, 0x1c(r31)
    b @L_801A6758
    @L_801A6754:
    li r3, 0x0
    @L_801A6758:
    bl fn_80191E88
    @L_801A675C:
    lwz r31, 0xc(r31)
    @L_801A6760:
    cmplwi r31, 0x0
    bne @L_801A6714
    @L_801A6768:
    lwz r30, 0xc(r30)
    @L_801A676C:
    cmplwi r30, 0x0
    bne @L_801A6704
    @L_801A6774:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
