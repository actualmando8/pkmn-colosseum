    nofralloc
    clrlslwi r0, r4, 24, 1
    or r0, r0, r6
    rlwinm r4, r0, 0, 26, 24
    rlwinm r0, r5, 6, 0, 25
    cmpwi r9, 0x0
    rlwinm r10, r3, 0, 30, 31
    or r6, r4, r0
    bne @L_800BA718
    li r8, 0x0
@L_800BA718:
    subfic r4, r9, 0x2
    subic r0, r4, 0x1
    subfe r5, r0, r4
    neg r4, r9
    subic r0, r4, 0x1
    subfe r0, r0, r4
    rlwinm r6, r6, 0, 25, 22
    rlwinm r4, r8, 7, 0, 24
    or r4, r6, r4
    rlwinm r6, r4, 0, 23, 21
    rlwinm r4, r5, 9, 0, 22
    or r4, r6, r4
    rlwinm r4, r4, 0, 22, 20
    rlwinm r0, r0, 10, 0, 21
    or r6, r4, r0
    rlwinm r6, r6, 0, 30, 25
    rlwimi r6, r7, 2, 26, 29
    rlwinm r6, r6, 0, 21, 16
    li r5, 0x10
    lis r4, 0xcc01
    stb r5, -0x8000(r4)
    addi r0, r10, 0x100e
    rlwimi r6, r7, 7, 17, 20
    stw r0, -0x8000(r4)
    cmpwi r3, 0x4
    stw r6, -0x8000(r4)
    bne @L_800BA798
    stb r5, -0x8000(r4)
    li r0, 0x1010
    stw r0, -0x8000(r4)
    stw r6, -0x8000(r4)
    b @L_800BA7B0
@L_800BA798:
    cmpwi r3, 0x5
    bne @L_800BA7B0
    stb r5, -0x8000(r4)
    li r0, 0x1011
    stw r0, -0x8000(r4)
    stw r6, -0x8000(r4)
@L_800BA7B0:
    lwz r3, -29632(r13)
    li r0, 0x1
    sth r0, 0x2(r3)
    blr
