    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8010223C
    lwz r31, 0x20(r3)
    b @L_80102234
    @L_80102224:
    mr r3, r31
    mr r4, r30
    bl fn_80109220
    lwz r31, 0x0(r31)
    @L_80102234:
    cmplwi r31, 0x0
    bne @L_80102224
    @L_8010223C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
