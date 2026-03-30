    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    lwz r3, -31600(r13)
    cmpwi r3, 0x0
    blt @L_801CA9E0
    rlwinm r4, r0, 0, 24, 31
    bl fn_8010C844
@L_801CA9E0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
