    nofralloc
    lwz r7, -31416(r13)
    rlwinm r8, r3, 0, 16, 31
    lis r5, -32713
    clrlslwi r6, r3, 16, 1
    cmplw r8, r7
    addi r0, r5, 23824
    add r5, r0, r6
    blt @L_8020E50C
    li r5, 0x0
@L_8020E50C:
    cmplwi r5, 0x0
    bne @L_8020E51C
    li r6, 0x0
    b @L_8020E52C
@L_8020E51C:
    bne @L_8020E528
    li r6, 0x0
    b @L_8020E52C
@L_8020E528:
    lbz r6, 0x0(r5)
@L_8020E52C:
    lis r5, -32713
    cmplw r8, r7
    clrlslwi r3, r3, 16, 1
    addi r0, r5, 23824
    add r5, r0, r3
    blt @L_8020E548
    li r5, 0x0
@L_8020E548:
    rlwinm r0, r6, 0, 24, 31
    cmplwi r5, 0x0
    mullw r3, r4, r0
    bne @L_8020E560
    li r0, 0x1
    b @L_8020E570
@L_8020E560:
    bne @L_8020E56C
    li r0, 0x1
    b @L_8020E570
@L_8020E56C:
    lbz r0, 0x1(r5)
@L_8020E570:
    rlwinm r0, r0, 0, 24, 31
    divwu r3, r3, r0
    blr
