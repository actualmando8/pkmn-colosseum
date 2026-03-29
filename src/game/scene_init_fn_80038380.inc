    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80105624
    mr r31, r3
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800383E0
    lwz r3, 0x4(r30)
    bl fn_801020C0
    lbz r4, 0x95(r30)
    extsb r5, r3
    addi r3, r4, 0x1
    extsb r0, r3
    stb r3, 0x95(r30)
    cmpw r0, r5
    blt @L_800383E0
    subi r0, r5, 0x1
    extsb r0, r0
    stb r0, 0x95(r30)
    @L_800383E0:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80038410
    lbz r3, 0x95(r30)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r30)
    cmpwi r0, 0x0
    bge @L_80038410
    li r0, 0x0
    stb r0, 0x95(r30)
    @L_80038410:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
