    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, -30880(r13)
    rlwinm r5, r3, 0, 16, 31
    lwz r0, 0x0(r4)
    cmplw r5, r0
    blt @L_80261414
    lis r3, -32728
    lis r4, -32710
    addi r3, r3, -23352
    addi r4, r4, -22840
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80261420
@L_80261414:
    lwz r3, -30876(r13)
    rlwinm r0, r5, 3, 0, 28
    add r3, r3, r0
@L_80261420:
    cmplwi r3, 0x0
    bne @L_80261430
    li r3, 0x0
    b @L_80261434
@L_80261430:
    lwz r3, 0x4(r3)
@L_80261434:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
