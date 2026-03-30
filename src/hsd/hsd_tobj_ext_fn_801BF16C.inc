    nofralloc
    lbz r7, 0x0(r3)
    lis r10, 0x8081
    lbz r0, 0x0(r4)
    lis r9, 0x8081
    lis r8, 0x8081
    lis r6, 0x8081
    mullw r0, r7, r0
    subi r10, r10, 0x7f7f
    subi r9, r9, 0x7f7f
    subi r8, r8, 0x7f7f
    subi r6, r6, 0x7f7f
    mulhwu r0, r10, r0
    extrwi r0, r0, 8, 17
    stb r0, 0x0(r5)
    lbz r7, 0x1(r3)
    lbz r0, 0x1(r4)
    mullw r0, r7, r0
    mulhwu r0, r9, r0
    extrwi r0, r0, 8, 17
    stb r0, 0x1(r5)
    lbz r7, 0x2(r3)
    lbz r0, 0x2(r4)
    mullw r0, r7, r0
    mulhwu r0, r8, r0
    extrwi r0, r0, 8, 17
    stb r0, 0x2(r5)
    lbz r3, 0x3(r3)
    lbz r0, 0x3(r4)
    mullw r0, r3, r0
    mulhwu r0, r6, r0
    extrwi r0, r0, 8, 17
    stb r0, 0x3(r5)
    blr
