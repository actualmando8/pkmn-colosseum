    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80166C74
    cmplwi r3, 0x1
    beq @L_80020B10
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80020B20
    @L_80020B10:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    @L_80020B20:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
