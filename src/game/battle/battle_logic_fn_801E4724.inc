    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32697
    stw r0, 0x14(r1)
    addi r3, r3, -21408
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    beq @L_801E4764
    lbz r0, 0xa4(r3)
    cmplwi r0, 0x0
    bne @L_801E4764
    li r0, 0x0
    stw r0, 0xa0(r3)
    bl fn_800A50E4
    li r3, 0x1
    b @L_801E4768
@L_801E4764:
    li r3, 0x0
@L_801E4768:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
