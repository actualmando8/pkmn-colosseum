    nofralloc
    lwz r3, -29632(r13)
    lhz r0, 0x4(r3)
    cmplwi r0, 0x0
    beqlr
    lwz r9, 0x14(r3)
    addi r5, r13, -32176
    lwz r8, 0x1c(r3)
    rlwinm r0, r9, 0, 31, 31
    lwz r4, 0x18(r3)
    rlwinm r6, r9, 31, 31, 31
    add r0, r0, r6
    rlwinm r6, r9, 30, 31, 31
    add r0, r0, r6
    rlwinm r7, r9, 29, 31, 31
    rlwinm r6, r9, 23, 30, 31
    lbzx r5, r5, r6
    rlwinm r8, r8, 23, 31, 31
    add r0, r0, r7
    rlwinm r6, r9, 28, 31, 31
    add r0, r0, r6
    rlwinm r6, r9, 27, 31, 31
    add r0, r0, r6
    rlwinm r6, r9, 26, 31, 31
    add r0, r0, r6
    rlwinm r6, r9, 25, 31, 31
    add r0, r0, r6
    rlwinm r6, r9, 24, 31, 31
    add r0, r0, r6
    cmpwi r8, 0x1
    add r0, r0, r5
    bne @L_800B7C9C
    li r8, 0x3
    b @L_800B7CA0
@L_800B7C9C:
    li r8, 0x1
@L_800B7CA0:
    rlwinm r6, r9, 21, 30, 31
    addi r5, r13, -32176
    lbzx r6, r5, r6
    rlwinm r5, r9, 19, 30, 31
    addi r7, r13, -32184
    mullw r8, r6, r8
    lbzx r6, r7, r5
    add r0, r0, r8
    rlwinm r5, r9, 17, 30, 31
    lbzx r9, r7, r5
    add r0, r0, r6
    rlwinm r6, r4, 0, 30, 31
    addi r8, r13, -32180
    rlwinm r5, r4, 30, 30, 31
    lbzx r7, r8, r6
    add r0, r0, r9
    lbzx r6, r8, r5
    add r0, r0, r7
    rlwinm r5, r4, 28, 30, 31
    lbzx r7, r8, r5
    add r0, r0, r6
    rlwinm r5, r4, 26, 30, 31
    lbzx r6, r8, r5
    add r0, r0, r7
    rlwinm r5, r4, 24, 30, 31
    lbzx r7, r8, r5
    add r0, r0, r6
    rlwinm r5, r4, 22, 30, 31
    lbzx r6, r8, r5
    rlwinm r5, r4, 20, 30, 31
    add r0, r0, r7
    lbzx r5, r8, r5
    rlwinm r4, r4, 18, 30, 31
    add r0, r0, r6
    lbzx r4, r8, r4
    add r0, r0, r5
    add r0, r0, r4
    sth r0, 0x6(r3)
    blr
