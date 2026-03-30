    nofralloc
    rlwinm r0, r4, 3, 0, 28
    add r6, r3, r0
    lwz r6, 0x38(r6)
    lbz r0, 0xe(r6)
    cmplwi r0, 0xff
    beq @L_801B5D30
    lbz r0, 0xe(r6)
    cmplwi r0, 0x4
    bge @L_801B5CC0
    li r3, -0x1
    blr
@L_801B5CC0:
    rlwinm r0, r4, 3, 0, 28
    li r7, 0x5
    add r5, r3, r0
    stb r7, 0x34(r5)
    lbz r0, 0xc(r6)
    cmplwi r0, 0x6
    bne @L_801B5D04
    lbz r5, 0xe(r6)
    rlwinm r0, r4, 3, 0, 28
    lis r6, -32713
    addi r6, r6, -11688
    rlwinm r4, r5, 2, 0, 29
    add r4, r6, r4
    add r3, r3, r0
    lwz r0, -0x10(r4)
    stb r0, 0x36(r3)
    b @L_801B5D28
@L_801B5D04:
    lbz r5, 0xe(r6)
    rlwinm r0, r4, 3, 0, 28
    lis r6, -32713
    addi r6, r6, -11672
    rlwinm r4, r5, 2, 0, 29
    add r4, r6, r4
    add r3, r3, r0
    lwz r0, -0x10(r4)
    stb r0, 0x36(r3)
@L_801B5D28:
    li r3, 0x0
    blr
@L_801B5D30:
    lbz r0, 0xc(r6)
    cmplwi r0, 0x6
    bne @L_801B5DBC
    li r7, 0x4
    b @L_801B5DB0
@L_801B5D44:
    rlwinm r0, r7, 1, 0, 30
    add r8, r5, r0
    lbz r0, 0xd(r8)
    cmplwi r0, 0x0
    bne @L_801B5DAC
    rlwinm r0, r7, 1, 0, 30
    rlwinm r8, r4, 3, 0, 28
    add r5, r5, r0
    li r9, 0x5
    li r0, 0x1
    add r8, r3, r8
    stb r0, 0xd(r5)
    rlwinm r0, r4, 3, 0, 28
    li r4, 0x3
    lis r5, -32713
    stb r7, 0xe(r6)
    stb r4, 0xf(r6)
    addi r6, r5, -11688
    rlwinm r5, r7, 2, 0, 29
    add r4, r3, r0
    stb r9, 0x34(r8)
    add r5, r6, r5
    li r3, 0x0
    lwz r0, -0x10(r5)
    stb r0, 0x36(r4)
    blr
@L_801B5DAC:
    addi r7, r7, 0x1
@L_801B5DB0:
    cmpwi r7, 0x8
    blt @L_801B5D44
    b @L_801B5E38
@L_801B5DBC:
    li r7, 0x4
    b @L_801B5E30
@L_801B5DC4:
    rlwinm r0, r7, 1, 0, 30
    add r8, r5, r0
    lbz r0, 0xc(r8)
    cmplwi r0, 0x0
    bne @L_801B5E2C
    rlwinm r0, r7, 1, 0, 30
    rlwinm r8, r4, 3, 0, 28
    add r5, r5, r0
    li r9, 0x5
    li r0, 0x3
    add r8, r3, r8
    stb r0, 0xc(r5)
    rlwinm r0, r4, 3, 0, 28
    li r4, 0x0
    lis r5, -32713
    stb r7, 0xe(r6)
    stb r4, 0xf(r6)
    addi r6, r5, -11672
    rlwinm r5, r7, 2, 0, 29
    add r4, r3, r0
    stb r9, 0x34(r8)
    add r5, r6, r5
    li r3, 0x0
    lwz r0, -0x10(r5)
    stb r0, 0x36(r4)
    blr
@L_801B5E2C:
    addi r7, r7, 0x1
@L_801B5E30:
    cmpwi r7, 0x8
    blt @L_801B5DC4
@L_801B5E38:
    li r3, -0x1
    blr
