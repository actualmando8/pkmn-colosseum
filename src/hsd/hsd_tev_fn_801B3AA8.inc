    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, -21700(r13)
    cmpw r0, r31
    beq @L_801B3AD4
    rlwinm r3, r31, 0, 24, 31
    bl fn_800BA6B0
    stw r31, -21700(r13)
@L_801B3AD4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
