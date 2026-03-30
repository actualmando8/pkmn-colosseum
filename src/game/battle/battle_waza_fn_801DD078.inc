    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x18(r3)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x4
    beq @L_801DD0B4
    bl fn_801DF3D4
    lbz r0, 0x18(r31)
    mr r3, r31
    ori r0, r0, 0x4
    stb r0, 0x18(r31)
    bl fn_801D9E8C
@L_801DD0B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
