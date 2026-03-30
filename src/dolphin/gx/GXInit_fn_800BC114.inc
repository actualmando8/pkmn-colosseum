    nofralloc
    cmpwi r3, 0x0
    lis r5, -32719
    addi r0, r5, 13712
    bne @L_800BC134
    rlwinm r4, r4, 2, 0, 29
    add r5, r0, r4
    addi r9, r5, 0x28
    b @L_800BC148
@L_800BC134:
    rlwinm r4, r4, 2, 0, 29
    add r5, r0, r4
    add r9, r0, r4
    addi r5, r5, 0x14
    addi r9, r9, 0x3c
@L_800BC148:
    lwz r7, -29632(r13)
    rlwinm r3, r3, 2, 0, 29
    lwz r0, 0x0(r5)
    li r5, 0x61
    add r6, r7, r3
    lwz r3, 0x130(r6)
    lis r4, 0xcc01
    stb r5, -0x8000(r4)
    rlwinm r3, r3, 0, 0, 7
    rlwimi r3, r0, 0, 8, 31
    stw r3, -0x8000(r4)
    li r0, 0x0
    stw r3, 0x130(r6)
    lwz r8, 0x170(r6)
    lwz r3, 0x0(r9)
    rlwinm r8, r8, 0, 28, 7
    stb r5, -0x8000(r4)
    rlwimi r8, r3, 0, 8, 27
    stw r8, -0x8000(r4)
    stw r8, 0x170(r6)
    sth r0, 0x2(r7)
    blr
