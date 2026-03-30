    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addis r3, r3, 0x1
    lbz r0, -0x3678(r3)
    cmplwi r0, 0x0
    beq @L_8006AF1C
    subi r3, r3, 0x4cd8
    b @L_8006AF20
    @L_8006AF1C:
    li r3, 0x0
    @L_8006AF20:
    cmplwi r3, 0x0
    bne @L_8006AF30
    li r3, 0x0
    b @L_8006AF34
    @L_8006AF30:
    addi r3, r3, 0xb44
    @L_8006AF34:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
