    nofralloc
    cmpwi r3, 0x2
    beq @L_800BBA48
    bge @L_800BB998
    cmpwi r3, 0x0
    beq @L_800BB9A4
    bge @L_800BB9F4
    b @L_800BBAE8
@L_800BB998:
    cmpwi r3, 0x4
    bge @L_800BBAE8
    b @L_800BBA98
@L_800BB9A4:
    lwz r7, -29632(r13)
    rlwinm r5, r5, 4, 0, 27
    li r0, 0x61
    lwz r6, 0x128(r7)
    lis r3, 0xcc01
    rlwinm r6, r6, 0, 0, 27
    or r4, r6, r4
    stw r4, 0x128(r7)
    lwz r4, 0x128(r7)
    rlwinm r4, r4, 0, 28, 23
    or r4, r4, r5
    stw r4, 0x128(r7)
    lwz r4, 0x128(r7)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x2500
    stw r4, 0x128(r7)
    stb r0, -0x8000(r3)
    lwz r0, 0x128(r7)
    stw r0, -0x8000(r3)
    b @L_800BBAE8
@L_800BB9F4:
    lwz r7, -29632(r13)
    rlwinm r6, r4, 8, 0, 23
    rlwinm r4, r5, 12, 0, 19
    lwz r5, 0x128(r7)
    li r0, 0x61
    lis r3, 0xcc01
    rlwinm r5, r5, 0, 24, 19
    or r5, r5, r6
    stw r5, 0x128(r7)
    lwz r5, 0x128(r7)
    rlwinm r5, r5, 0, 20, 15
    or r4, r5, r4
    stw r4, 0x128(r7)
    lwz r4, 0x128(r7)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x2500
    stw r4, 0x128(r7)
    stb r0, -0x8000(r3)
    lwz r0, 0x128(r7)
    stw r0, -0x8000(r3)
    b @L_800BBAE8
@L_800BBA48:
    lwz r7, -29632(r13)
    rlwinm r5, r5, 4, 0, 27
    li r0, 0x61
    lwz r6, 0x12c(r7)
    lis r3, 0xcc01
    rlwinm r6, r6, 0, 0, 27
    or r4, r6, r4
    stw r4, 0x12c(r7)
    lwz r4, 0x12c(r7)
    rlwinm r4, r4, 0, 28, 23
    or r4, r4, r5
    stw r4, 0x12c(r7)
    lwz r4, 0x12c(r7)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x2600
    stw r4, 0x12c(r7)
    stb r0, -0x8000(r3)
    lwz r0, 0x12c(r7)
    stw r0, -0x8000(r3)
    b @L_800BBAE8
@L_800BBA98:
    lwz r7, -29632(r13)
    rlwinm r6, r4, 8, 0, 23
    rlwinm r4, r5, 12, 0, 19
    lwz r5, 0x12c(r7)
    li r0, 0x61
    lis r3, 0xcc01
    rlwinm r5, r5, 0, 24, 19
    or r5, r5, r6
    stw r5, 0x12c(r7)
    lwz r5, 0x12c(r7)
    rlwinm r5, r5, 0, 20, 15
    or r4, r5, r4
    stw r4, 0x12c(r7)
    lwz r4, 0x12c(r7)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x2600
    stw r4, 0x12c(r7)
    stb r0, -0x8000(r3)
    lwz r0, 0x12c(r7)
    stw r0, -0x8000(r3)
@L_800BBAE8:
    lwz r3, -29632(r13)
    li r0, 0x0
    sth r0, 0x2(r3)
    blr
