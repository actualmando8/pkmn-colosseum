    nofralloc
    cmpwi r3, 0x2
    beq @L_800BBB80
    bge @L_800BBB14
    cmpwi r3, 0x0
    beq @L_800BBB20
    bge @L_800BBB50
    b @L_800BBBDC
@L_800BBB14:
    cmpwi r3, 0x4
    bge @L_800BBBDC
    b @L_800BBBB0
@L_800BBB20:
    lwz r3, -29632(r13)
    rlwinm r0, r4, 3, 0, 28
    addi r4, r3, 0x120
    lwz r3, 0x120(r3)
    rlwinm r3, r3, 0, 0, 28
    or r3, r3, r5
    stw r3, 0x0(r4)
    lwz r3, 0x0(r4)
    rlwinm r3, r3, 0, 29, 25
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800BBBDC
@L_800BBB50:
    lwz r6, -29632(r13)
    rlwinm r0, r4, 9, 0, 22
    rlwinm r3, r5, 6, 0, 25
    lwz r4, 0x120(r6)
    rlwinm r4, r4, 0, 26, 22
    or r3, r4, r3
    stw r3, 0x120(r6)
    lwz r3, 0x120(r6)
    rlwinm r3, r3, 0, 23, 19
    or r0, r3, r0
    stw r0, 0x120(r6)
    b @L_800BBBDC
@L_800BBB80:
    lwz r6, -29632(r13)
    rlwinm r0, r4, 15, 0, 16
    rlwinm r3, r5, 12, 0, 19
    lwz r4, 0x120(r6)
    rlwinm r4, r4, 0, 20, 16
    or r3, r4, r3
    stw r3, 0x120(r6)
    lwz r3, 0x120(r6)
    rlwinm r3, r3, 0, 17, 13
    or r0, r3, r0
    stw r0, 0x120(r6)
    b @L_800BBBDC
@L_800BBBB0:
    lwz r6, -29632(r13)
    rlwinm r0, r4, 21, 0, 10
    rlwinm r3, r5, 18, 0, 13
    lwz r4, 0x120(r6)
    rlwinm r4, r4, 0, 14, 10
    or r3, r4, r3
    stw r3, 0x120(r6)
    lwz r3, 0x120(r6)
    rlwinm r3, r3, 0, 11, 7
    or r0, r3, r0
    stw r0, 0x120(r6)
@L_800BBBDC:
    li r0, 0x61
    lwz r4, -29632(r13)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x0
    lwz r3, 0x120(r4)
    stw r3, -0x8000(r5)
    lwz r3, 0x4f4(r4)
    ori r3, r3, 0x3
    stw r3, 0x4f4(r4)
    sth r0, 0x2(r4)
    blr
