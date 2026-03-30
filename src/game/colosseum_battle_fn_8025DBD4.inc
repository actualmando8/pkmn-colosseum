    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, -31256(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_8025DC10
    lis r3, -32728
    lis r4, -32710
    addi r3, r3, -23472
    addi r4, r4, -22896
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8025DC1C
@L_8025DC10:
    lwz r4, -31252(r13)
    rlwinm r0, r3, 2, 0, 29
    lwzx r3, r4, r0
@L_8025DC1C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
