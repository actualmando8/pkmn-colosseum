    nofralloc
    stwu r1, -0x20(r1)
    lis r5, lbl_803A654C@ha
    addi r6, r5, lbl_803A654C@l
    lfs f0, 0x40(r6)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r5, 0xc(r1)
    extsh r0, r5
    cmpwi r0, -0xff
    bge @L_80037494
    li r5, -0xff
    @L_80037494:
    addi r0, r5, 0xff
    lis r5, lbl_803A64EC@ha
    clrlwi r0, r0, 24
    stb r0, 0x67(r4)
    addi r5, r5, lbl_803A64EC@l
    lfs f1, 0x3c(r5)
    lfs f0, 0x40(r6)
    fadds f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x50(r4)
    lbz r0, 0x95(r3)
    extsb r0, r0
    cmpwi r0, 0x2
    bne @L_800374E8
    li r0, 0xff
    stb r0, 0x64(r4)
    stb r0, 0x65(r4)
    stb r0, 0x66(r4)
    b @L_80037500
    @L_800374E8:
    li r0, 0x46
    li r3, 0x8f
    stb r0, 0x64(r4)
    li r0, 0xb4
    stb r3, 0x65(r4)
    stb r0, 0x66(r4)
    @L_80037500:
    addi r1, r1, 0x20
    blr
