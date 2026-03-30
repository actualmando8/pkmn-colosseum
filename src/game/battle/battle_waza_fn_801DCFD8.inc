    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r3, 0x18(r3)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DD014
    ori r0, r3, 0x8
    stb r0, 0x18(r31)
    lwz r3, 0x24(r31)
    bl fn_800EC96C
    mr r3, r31
    bl fn_801DA070
@L_801DD014:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
