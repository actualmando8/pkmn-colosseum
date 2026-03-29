    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0x60(r30)
    bl fn_80105624
    lhz r0, 0x6(r3)
    lwz r5, 0xc(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80019634
    lbz r0, 0x95(r30)
    extsb r4, r0
    addi r0, r4, 0x1
    cmpw r0, r5
    blt @L_8001962C
    subi r0, r5, 0x1
    @L_8001962C:
    extsb r0, r0
    stb r0, 0x95(r30)
    @L_80019634:
    lhz r0, 0x6(r3)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80019664
    lbz r0, 0x95(r30)
    extsb r3, r0
    subi r0, r3, 0x1
    cmpwi r0, 0x0
    bge @L_8001965C
    li r0, 0x0
    @L_8001965C:
    extsb r0, r0
    stb r0, 0x95(r30)
    @L_80019664:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
