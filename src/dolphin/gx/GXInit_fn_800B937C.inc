    nofralloc
    lwz r3, -29632(r13)
    li r0, 0x98
    lis r5, 0xcc01
    lhz r6, 0x4(r3)
    li r4, 0x0
    lhz r3, 0x6(r3)
    mullw r7, r6, r3
    stb r0, -0x8000(r5)
    sth r6, -0x8000(r5)
    addi r3, r7, 0x3
    cmplwi r7, 0x0
    rlwinm r3, r3, 30, 2, 31
    ble @L_800B93F4
    rlwinm. r0, r3, 29, 3, 31
    mtctr r0
    beq @L_800B93E8
@L_800B93BC:
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    stw r4, -0x8000(r5)
    bdnz @L_800B93BC
    andi. r3, r3, 0x7
    beq @L_800B93F4
@L_800B93E8:
    mtctr r3
@L_800B93EC:
    stw r4, -0x8000(r5)
    bdnz @L_800B93EC
@L_800B93F4:
    lwz r3, -29632(r13)
    li r0, 0x1
    sth r0, 0x2(r3)
    blr
