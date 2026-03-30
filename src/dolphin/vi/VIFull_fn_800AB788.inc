    nofralloc
    li r3, 0x0
    sth r3, 0x0(r4)
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 16, 24, 24
    beq @L_800AB7A0
    li r3, 0x100
    @L_800AB7A0:
    lhz r0, 0x0(r4)
    or r0, r0, r3
    sth r0, 0x0(r4)
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 16, 23, 23
    beq @L_800AB7C0
    li r3, 0x200
    b @L_800AB7C4
    @L_800AB7C0:
    li r3, 0x0
    @L_800AB7C4:
    lhz r0, 0x0(r4)
    or r0, r0, r3
    sth r0, 0x0(r4)
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 16, 26, 26
    beq @L_800AB7E4
    li r3, 0x400
    b @L_800AB7E8
    @L_800AB7E4:
    li r3, 0x0
    @L_800AB7E8:
    lhz r0, 0x0(r4)
    or r0, r0, r3
    sth r0, 0x0(r4)
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 16, 27, 27
    beq @L_800AB808
    li r3, 0x800
    b @L_800AB80C
    @L_800AB808:
    li r3, 0x0
    @L_800AB80C:
    lhz r0, 0x0(r4)
    or r0, r0, r3
    sth r0, 0x0(r4)
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 16, 22, 22
    beq @L_800AB82C
    li r6, 0x1000
    b @L_800AB830
    @L_800AB82C:
    li r6, 0x0
    @L_800AB830:
    lhz r3, 0x0(r4)
    li r0, 0x0
    or r3, r3, r6
    sth r3, 0x0(r4)
    lwz r3, 0x4(r5)
    srwi r3, r3, 16
    extsb r3, r3
    stb r3, 0x2(r4)
    lwz r3, 0x4(r5)
    srwi r3, r3, 24
    extsb r3, r3
    stb r3, 0x3(r4)
    lwz r3, 0x4(r5)
    extsb r3, r3
    stb r3, 0x4(r4)
    lwz r3, 0x4(r5)
    srwi r3, r3, 8
    extsb r3, r3
    stb r3, 0x5(r4)
    lwz r3, 0x0(r5)
    extrwi r3, r3, 8, 16
    stb r3, 0x6(r4)
    lwz r3, 0x0(r5)
    stb r3, 0x7(r4)
    stb r0, 0x8(r4)
    stb r0, 0x9(r4)
    lbz r0, 0x6(r4)
    cmplwi r0, 0xaa
    blt @L_800AB8B0
    lhz r0, 0x0(r4)
    ori r0, r0, 0x40
    sth r0, 0x0(r4)
    @L_800AB8B0:
    lbz r0, 0x7(r4)
    cmplwi r0, 0xaa
    blt @L_800AB8C8
    lhz r0, 0x0(r4)
    ori r0, r0, 0x20
    sth r0, 0x0(r4)
    @L_800AB8C8:
    lbz r3, 0x2(r4)
    subi r0, r3, 0x80
    stb r0, 0x2(r4)
    lbz r3, 0x3(r4)
    subi r0, r3, 0x80
    stb r0, 0x3(r4)
    lbz r3, 0x4(r4)
    subi r0, r3, 0x80
    stb r0, 0x4(r4)
    lbz r3, 0x5(r4)
    subi r0, r3, 0x80
    stb r0, 0x5(r4)
    blr
