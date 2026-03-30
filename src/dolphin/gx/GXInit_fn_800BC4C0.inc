    nofralloc
    srawi r5, r3, 1
    lwz r6, -29632(r13)
    rlwinm. r0, r3, 0, 31, 31
    rlwinm r3, r5, 2, 0, 29
    addi r7, r3, 0x1b0
    add r7, r6, r7
    beq @L_800BC4F4
    lwz r3, 0x0(r7)
    rlwinm r0, r4, 19, 0, 12
    rlwinm r3, r3, 0, 13, 7
    or r0, r3, r0
    stw r0, 0x0(r7)
    b @L_800BC508
@L_800BC4F4:
    lwz r3, 0x0(r7)
    rlwinm r0, r4, 9, 0, 22
    rlwinm r3, r3, 0, 23, 17
    or r0, r3, r0
    stw r0, 0x0(r7)
@L_800BC508:
    li r0, 0x61
    lwz r3, -29632(r13)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x0
    lwz r4, 0x0(r7)
    stw r4, -0x8000(r5)
    sth r0, 0x2(r3)
    blr
