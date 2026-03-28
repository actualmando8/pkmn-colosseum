    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_801ED2B4
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED2B4:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801ED2C8
    lbz r3, 0x1(r3)
    b @L_801ED2CC
@L_801ED2C8:
    li r3, 0xff
@L_801ED2CC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
