    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, -32709
    addi r5, r4, 28040
    lwz r4, 0x40(r5)
    cmplwi r4, 0x8
    blt @L_800715F8
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30968
    li r4, 0x41
    addi r5, r5, -30896
    bl fn_80196E10
    b @L_8007161C
@L_800715F8:
    addi r0, r4, 0x1
    li r4, 0x0
    stw r0, 0x40(r5)
    rlwinm r0, r0, 3, 0, 28
    stwx r3, r5, r0
    lwz r0, 0x40(r5)
    rlwinm r0, r0, 3, 0, 28
    add r3, r5, r0
    stw r4, 0x4(r3)
@L_8007161C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
