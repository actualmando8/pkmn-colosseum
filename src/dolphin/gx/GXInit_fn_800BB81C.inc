    nofralloc
    cmpwi r3, 0x8
    stwu r1, -0x78(r1)
    beq @L_800BB868
    bge @L_800BB844
    cmpwi r3, 0x4
    beq @L_800BB868
    bge @L_800BB858
    cmpwi r3, 0x1
    bge @L_800BB850
    b @L_800BB868
@L_800BB844:
    cmpwi r3, 0xc
    bge @L_800BB868
    b @L_800BB860
@L_800BB850:
    subi r0, r3, 0x1
    b @L_800BB86C
@L_800BB858:
    subi r0, r3, 0x5
    b @L_800BB86C
@L_800BB860:
    subi r0, r3, 0x9
    b @L_800BB86C
@L_800BB868:
    li r0, 0x0
@L_800BB86C:
    lfs f2, -29488(r2)
    mulli r3, r0, 0x3
    lfs f1, 0x0(r4)
    lfs f0, 0xc(r4)
    fmuls f1, f2, f1
    addi r11, r5, 0x11
    fmuls f0, f2, f0
    addi r0, r3, 0x6
    extsb r11, r11
    fctiwz f1, f1
    li r10, 0x61
    fctiwz f0, f0
    lis r9, 0xcc01
    stb r10, -0x8000(r9)
    stfd f1, 0x70(r1)
    rlwinm r0, r0, 24, 0, 7
    addi r6, r3, 0x7
    stfd f0, 0x68(r1)
    addi r5, r3, 0x8
    lwz r8, 0x74(r1)
    lwz r7, 0x6c(r1)
    lwz r3, -29632(r13)
    clrlslwi r7, r7, 21, 11
    rlwimi r7, r8, 0, 21, 31
    rlwinm r7, r7, 0, 10, 7
    rlwimi r7, r11, 22, 8, 9
    rlwimi r0, r7, 0, 8, 31
    stw r0, -0x8000(r9)
    li r0, 0x0
    lfs f1, 0x4(r4)
    lfs f0, 0x10(r4)
    fmuls f1, f2, f1
    fmuls f0, f2, f0
    stb r10, -0x8000(r9)
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x60(r1)
    stfd f0, 0x58(r1)
    lwz r8, 0x64(r1)
    lwz r7, 0x5c(r1)
    clrlslwi r7, r7, 21, 11
    rlwimi r7, r8, 0, 21, 31
    rlwinm r7, r7, 0, 10, 7
    rlwimi r7, r11, 20, 8, 9
    rlwinm r7, r7, 0, 8, 31
    rlwimi r7, r6, 24, 0, 7
    stw r7, -0x8000(r9)
    lfs f1, 0x8(r4)
    lfs f0, 0x14(r4)
    fmuls f1, f2, f1
    fmuls f0, f2, f0
    stb r10, -0x8000(r9)
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x50(r1)
    stfd f0, 0x48(r1)
    lwz r6, 0x54(r1)
    lwz r4, 0x4c(r1)
    clrlslwi r4, r4, 21, 11
    rlwimi r4, r6, 0, 21, 31
    rlwinm r4, r4, 0, 10, 7
    rlwimi r4, r11, 18, 8, 9
    rlwinm r4, r4, 0, 8, 31
    rlwimi r4, r5, 24, 0, 7
    stw r4, -0x8000(r9)
    sth r0, 0x2(r3)
    addi r1, r1, 0x78
    blr
