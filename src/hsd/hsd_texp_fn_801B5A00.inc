    nofralloc
    mr r7, r3
    rlwinm r0, r4, 3, 0, 28
    add r3, r7, r0
    lwz r6, 0x38(r3)
    lbz r0, 0xe(r6)
    cmplwi r0, 0xff
    beq @L_801B5ACC
    lbz r0, 0xe(r6)
    cmplwi r0, 0x4
    blt @L_801B5A30
    li r3, -0x1
    blr
@L_801B5A30:
    lbz r0, 0xc(r6)
    cmplwi r0, 0x6
    bne @L_801B5A88
    lbz r8, 0xe(r6)
    lis r5, -32713
    lbz r0, 0xf(r6)
    addi r5, r5, -11640
    rlwinm r6, r8, 4, 0, 27
    rlwinm r3, r4, 3, 0, 28
    add r5, r5, r6
    rlwinm r0, r0, 2, 0, 29
    add r5, r5, r0
    lwz r6, 0x0(r5)
    rlwinm r0, r4, 3, 0, 28
    add r4, r7, r3
    li r5, 0x6
    stb r6, 0x20(r7)
    add r3, r7, r0
    li r0, 0xe
    stb r5, 0x34(r4)
    stb r0, 0x36(r3)
    b @L_801B5AC4
@L_801B5A88:
    lbz r6, 0xe(r6)
    lis r5, -32713
    rlwinm r3, r4, 3, 0, 28
    rlwinm r0, r4, 3, 0, 28
    rlwinm r6, r6, 2, 0, 29
    addi r4, r5, -11576
    add r5, r4, r6
    lwz r6, 0x0(r5)
    add r4, r7, r3
    add r3, r7, r0
    li r5, 0x6
    stb r6, 0x20(r7)
    li r0, 0xe
    stb r5, 0x34(r4)
    stb r0, 0x36(r3)
@L_801B5AC4:
    li r3, 0x0
    blr
@L_801B5ACC:
    lbz r0, 0xc(r6)
    cmplwi r0, 0x6
    bne @L_801B5C04
    li r3, 0x1
    b @L_801B5B64
@L_801B5AE0:
    rlwinm r0, r3, 1, 0, 30
    add r8, r5, r0
    lbz r0, 0xd(r8)
    cmplwi r0, 0x0
    bne @L_801B5B60
    rlwinm r0, r3, 1, 0, 30
    lis r8, -32713
    add r5, r5, r0
    li r0, 0x1
    addi r9, r8, -11640
    stb r0, 0xd(r5)
    rlwinm r5, r4, 3, 0, 28
    rlwinm r0, r4, 3, 0, 28
    li r8, 0x6
    stb r3, 0xe(r6)
    li r3, 0x3
    add r4, r7, r0
    add r5, r7, r5
    stb r3, 0xf(r6)
    li r0, 0xe
    li r3, 0x0
    lbz r10, 0xe(r6)
    lbz r6, 0xf(r6)
    rlwinm r10, r10, 4, 0, 27
    add r9, r9, r10
    rlwinm r6, r6, 2, 0, 29
    add r6, r9, r6
    lwz r6, 0x0(r6)
    stb r6, 0x20(r7)
    stb r8, 0x34(r5)
    stb r0, 0x36(r4)
    blr
@L_801B5B60:
    addi r3, r3, 0x1
@L_801B5B64:
    cmpwi r3, 0x4
    blt @L_801B5AE0
    li r3, 0x0
    b @L_801B5BF8
@L_801B5B74:
    rlwinm r0, r3, 1, 0, 30
    add r8, r5, r0
    lbz r0, 0xc(r8)
    cmplwi r0, 0x3
    bge @L_801B5BF4
    rlwinm r0, r3, 1, 0, 30
    stb r3, 0xe(r6)
    add r8, r5, r0
    lis r5, -32713
    lbz r10, 0xc(r8)
    rlwinm r3, r4, 3, 0, 28
    addi r9, r5, -11640
    rlwinm r0, r4, 3, 0, 28
    add r5, r7, r3
    addi r3, r10, 0x1
    stb r3, 0xc(r8)
    add r4, r7, r0
    li r8, 0x6
    li r0, 0xe
    stb r10, 0xf(r6)
    li r3, 0x0
    lbz r10, 0xe(r6)
    lbz r6, 0xf(r6)
    rlwinm r10, r10, 4, 0, 27
    add r9, r9, r10
    rlwinm r6, r6, 2, 0, 29
    add r6, r9, r6
    lwz r6, 0x0(r6)
    stb r6, 0x20(r7)
    stb r8, 0x34(r5)
    stb r0, 0x36(r4)
    blr
@L_801B5BF4:
    addi r3, r3, 0x1
@L_801B5BF8:
    cmpwi r3, 0x4
    blt @L_801B5B74
    b @L_801B5C8C
@L_801B5C04:
    li r3, 0x0
    b @L_801B5C84
@L_801B5C0C:
    rlwinm r0, r3, 1, 0, 30
    add r8, r5, r0
    lbz r0, 0xc(r8)
    cmplwi r0, 0x0
    bne @L_801B5C80
    rlwinm r0, r3, 1, 0, 30
    lis r8, -32713
    add r5, r5, r0
    li r0, 0x3
    addi r9, r8, -11576
    stb r0, 0xc(r5)
    rlwinm r5, r4, 3, 0, 28
    rlwinm r0, r4, 3, 0, 28
    li r8, 0x6
    stb r3, 0xe(r6)
    li r3, 0x0
    add r4, r7, r0
    add r5, r7, r5
    stb r3, 0xf(r6)
    li r0, 0xe
    li r3, 0x0
    lbz r6, 0xe(r6)
    rlwinm r6, r6, 2, 0, 29
    add r6, r9, r6
    lwz r6, 0x0(r6)
    stb r6, 0x20(r7)
    stb r8, 0x34(r5)
    stb r0, 0x36(r4)
    blr
@L_801B5C80:
    addi r3, r3, 0x1
@L_801B5C84:
    cmpwi r3, 0x4
    blt @L_801B5C0C
@L_801B5C8C:
    li r3, -0x1
    blr
