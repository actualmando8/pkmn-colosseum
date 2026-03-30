    nofralloc
    mr r7, r3
    rlwinm r0, r4, 3, 0, 28
    add r3, r7, r0
    lwz r6, 0x58(r3)
    lbz r0, 0xe(r6)
    cmplwi r0, 0xff
    beq @L_801B58D0
    lbz r0, 0xe(r6)
    cmplwi r0, 0x4
    blt @L_801B5880
    li r3, -0x1
    blr
@L_801B5880:
    lbz r8, 0xe(r6)
    lis r5, -32713
    lbz r0, 0xf(r6)
    addi r5, r5, -11560
    rlwinm r6, r8, 4, 0, 27
    rlwinm r3, r4, 3, 0, 28
    add r5, r5, r6
    rlwinm r0, r0, 2, 0, 29
    add r5, r5, r0
    li r6, 0x6
    lwz r8, 0x0(r5)
    add r5, r7, r3
    rlwinm r0, r4, 3, 0, 28
    li r3, 0x0
    stb r8, 0x21(r7)
    add r4, r7, r0
    li r0, 0x6
    stb r6, 0x54(r5)
    stb r0, 0x56(r4)
    blr
@L_801B58D0:
    li r9, 0x1
    b @L_801B595C
@L_801B58D8:
    rlwinm r0, r9, 1, 0, 30
    add r3, r5, r0
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_801B5958
    rlwinm r0, r9, 1, 0, 30
    lis r8, -32713
    add r3, r5, r0
    li r0, 0x1
    stb r0, 0xd(r3)
    rlwinm r3, r4, 3, 0, 28
    rlwinm r0, r4, 3, 0, 28
    li r4, 0x3
    stb r9, 0xe(r6)
    addi r9, r8, -11560
    add r5, r7, r3
    li r8, 0x6
    stb r4, 0xf(r6)
    add r4, r7, r0
    li r0, 0x6
    li r3, 0x0
    lbz r10, 0xe(r6)
    lbz r6, 0xf(r6)
    rlwinm r10, r10, 4, 0, 27
    add r9, r9, r10
    rlwinm r6, r6, 2, 0, 29
    add r6, r9, r6
    lwz r6, 0x0(r6)
    stb r6, 0x21(r7)
    stb r8, 0x54(r5)
    stb r0, 0x56(r4)
    blr
@L_801B5958:
    addi r9, r9, 0x1
@L_801B595C:
    cmpwi r9, 0x4
    blt @L_801B58D8
    li r9, 0x0
    b @L_801B59F0
@L_801B596C:
    rlwinm r0, r9, 1, 0, 30
    add r3, r5, r0
    lbz r0, 0xc(r3)
    cmplwi r0, 0x3
    bge @L_801B59EC
    rlwinm r0, r9, 1, 0, 30
    stb r9, 0xe(r6)
    add r8, r5, r0
    lis r5, -32713
    lbz r10, 0xc(r8)
    rlwinm r3, r4, 3, 0, 28
    addi r9, r5, -11560
    rlwinm r0, r4, 3, 0, 28
    add r5, r7, r3
    addi r3, r10, 0x1
    stb r3, 0xc(r8)
    add r4, r7, r0
    li r8, 0x6
    li r0, 0x6
    stb r10, 0xf(r6)
    li r3, 0x0
    lbz r10, 0xe(r6)
    lbz r6, 0xf(r6)
    rlwinm r10, r10, 4, 0, 27
    add r9, r9, r10
    rlwinm r6, r6, 2, 0, 29
    add r6, r9, r6
    lwz r6, 0x0(r6)
    stb r6, 0x21(r7)
    stb r8, 0x54(r5)
    stb r0, 0x56(r4)
    blr
@L_801B59EC:
    addi r9, r9, 0x1
@L_801B59F0:
    cmpwi r9, 0x4
    blt @L_801B596C
    li r3, -0x1
    blr
