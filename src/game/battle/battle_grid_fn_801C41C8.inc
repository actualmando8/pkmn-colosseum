    nofralloc
    lis r4, lbl_80466E30@ha
    clrlwi r3, r3, 16
    addi r4, r4, lbl_80466E30@l
    li r5, 0x1
    stb r5, 0x1(r4)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    sth r3, 0x2(r4)
    stfs f1, 0x4(r4)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x8(r4)
    beq @L_801C421C
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_801C4210
    li r0, 0x4
    stb r0, 0x0(r4)
    blr
    @L_801C4210:
    li r0, 0x3
    stb r0, 0x0(r4)
    blr
    @L_801C421C:
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_801C4234
    li r0, 0x2
    stb r0, 0x0(r4)
    blr
    @L_801C4234:
    stb r5, 0x0(r4)
    blr
