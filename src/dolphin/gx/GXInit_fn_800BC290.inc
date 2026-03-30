    nofralloc
    lwz r0, -29632(r13)
    rlwinm r9, r3, 2, 0, 29
    cmpwi r4, 0x1
    add r3, r0, r9
    lwz r3, 0x170(r3)
    rlwimi r3, r4, 18, 13, 13
    addi r10, r3, 0x0
    bgt @L_800BC2BC
    rlwimi r10, r6, 20, 10, 11
    rlwimi r10, r5, 16, 14, 15
    b @L_800BC2C8
@L_800BC2BC:
    li r0, 0x3
    rlwimi r10, r4, 19, 10, 11
    rlwimi r10, r0, 16, 14, 15
@L_800BC2C8:
    li r0, 0x61
    lwz r4, -29632(r13)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    rlwimi r10, r7, 19, 12, 12
    rlwimi r10, r8, 22, 8, 9
    stw r10, -0x8000(r5)
    add r3, r4, r9
    li r0, 0x0
    stw r10, 0x170(r3)
    sth r0, 0x2(r4)
    blr
