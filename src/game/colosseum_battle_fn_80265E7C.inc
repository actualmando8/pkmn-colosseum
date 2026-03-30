    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x1
    stw r0, 0x14(r1)
    bl fn_8011D5F8
    li r3, 0x0
    li r4, 0x10
    bl fn_80129280
    lbz r0, 0x4(r3)
    cmplwi r0, 0x0
    bne @L_80265EB0
    li r0, 0x2c
    stb r0, 0x4(r3)
@L_80265EB0:
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
