    nofralloc
    cmpwi r3, 0x3
    beq @L_800BA554
    bge @L_800BA4EC
    cmpwi r3, 0x1
    beq @L_800BA51C
    bge @L_800BA53C
    cmpwi r3, 0x0
    bge @L_800BA4FC
    blr
@L_800BA4EC:
    cmpwi r3, 0x5
    beq @L_800BA578
    bgelr
    b @L_800BA56C
@L_800BA4FC:
    lwz r3, -29632(r13)
    li r5, 0x0
    lwz r0, 0x0(r4)
    lwz r3, 0xa8(r3)
    rlwinm r0, r0, 0, 0, 23
    mr r7, r0
    rlwimi r7, r3, 0, 24, 31
    b @L_800BA588
@L_800BA51C:
    lwz r3, -29632(r13)
    li r5, 0x1
    lwz r0, 0x0(r4)
    lwz r3, 0xac(r3)
    rlwinm r0, r0, 0, 0, 23
    mr r7, r0
    rlwimi r7, r3, 0, 24, 31
    b @L_800BA588
@L_800BA53C:
    lwz r3, -29632(r13)
    li r5, 0x0
    lbz r7, 0x3(r4)
    lwz r3, 0xa8(r3)
    rlwimi r7, r3, 0, 0, 23
    b @L_800BA588
@L_800BA554:
    lwz r3, -29632(r13)
    li r5, 0x1
    lbz r7, 0x3(r4)
    lwz r3, 0xac(r3)
    rlwimi r7, r3, 0, 0, 23
    b @L_800BA588
@L_800BA56C:
    lwz r7, 0x0(r4)
    li r5, 0x0
    b @L_800BA588
@L_800BA578:
    lwz r7, 0x0(r4)
    li r5, 0x1
    b @L_800BA588
    blr
@L_800BA588:
    li r0, 0x10
    lwz r4, -29632(r13)
    lis r6, 0xcc01
    stb r0, -0x8000(r6)
    addi r3, r5, 0x100a
    rlwinm r0, r5, 2, 0, 29
    stw r3, -0x8000(r6)
    li r5, 0x1
    add r3, r4, r0
    stw r7, -0x8000(r6)
    sth r5, 0x2(r4)
    stw r7, 0xa8(r3)
    blr
