    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmpwi r30, 0x0
    blt @L_8006B220
    cmpwi r30, 0x7
    blt @L_8006B228
    @L_8006B220:
    li r0, 0x0
    b @L_8006B240
    @L_8006B228:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addis r0, r30, 0x1
    add r3, r0, r3
    lbz r0, -0x342c(r3)
    @L_8006B240:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_8006B254
    li r3, 0x0
    b @L_8006B28C
    @L_8006B254:
    cmpwi r31, 0x0
    blt @L_8006B264
    cmplwi r31, 0x2
    blt @L_8006B26C
    @L_8006B264:
    li r3, 0x0
    b @L_8006B28C
    @L_8006B26C:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addis r4, r31, 0x1
    slwi r0, r30, 1
    add r3, r4, r3
    add r3, r3, r0
    lbz r3, -0x3425(r3)
    @L_8006B28C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
