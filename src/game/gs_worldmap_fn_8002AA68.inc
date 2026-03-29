    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_8002AAC4
    bge @L_8002AAE8
    cmpwi r0, 0x0
    beq @L_8002AA9C
    b @L_8002AAE8
    @L_8002AA9C:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8002AAE8
    li r3, 0x61
    li r4, 0x7e
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    b @L_8002AAE8
    @L_8002AAC4:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8002AAE8
    li r3, 0x61
    li r4, 0x82
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8002AAE8:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
