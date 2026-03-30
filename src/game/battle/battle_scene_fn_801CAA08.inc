    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0x0
    blt @L_801CAA24
    rlwinm r4, r4, 0, 24, 31
    bl fn_8010C844
@L_801CAA24:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
