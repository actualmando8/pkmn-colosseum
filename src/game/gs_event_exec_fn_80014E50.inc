    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    lwz r30, 0x60(r31)
    bl fn_80105624
    lhz r0, 0x6(r3)
    lwz r7, 0x8(r30)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80014ED0
    lbz r6, 0x95(r31)
    extsb r6, r6
    addi r8, r6, 0x1
    cmpw r8, r7
    blt @L_80014E9C
    subi r8, r7, 0x1
    @L_80014E9C:
    mulli r4, r8, 0xc
    lwz r5, 0x4(r30)
    addi r0, r4, 0x8
    lhzx r0, r5, r0
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80014EC8
    addi r8, r8, 0x1
    cmpw r8, r7
    blt @L_80014EC8
    mr r8, r6
    @L_80014EC8:
    extsb r0, r8
    stb r0, 0x95(r31)
    @L_80014ED0:
    lhz r0, 0x6(r3)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80014F2C
    lbz r5, 0x95(r31)
    extsb r5, r5
    subi r6, r5, 0x1
    cmpwi r6, 0x0
    bge @L_80014EF8
    li r6, 0x0
    @L_80014EF8:
    mulli r3, r6, 0xc
    lwz r4, 0x4(r30)
    addi r0, r3, 0x8
    lhzx r0, r4, r0
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80014F24
    subi r6, r6, 0x1
    cmpwi r6, 0x0
    bge @L_80014F24
    mr r6, r5
    @L_80014F24:
    extsb r0, r6
    stb r0, 0x95(r31)
    @L_80014F2C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
