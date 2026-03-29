    nofralloc
    stwu r1, -0x10(r1)
    lis r3, lbl_803A654C@ha
    lfs f0, lbl_8047BA58(r2)
    addi r5, r3, lbl_803A654C@l
    lfs f1, 0x58(r5)
    fcmpo cr0, f1, f0
    bge @L_800377AC
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    extsh r0, r3
    cmpwi r0, -0xff
    bge @L_80037730
    li r3, -0xff
    @L_80037730:
    addi r0, r3, 0xff
    lis r3, lbl_803A654C@ha
    clrlwi r0, r0, 24
    lfs f1, lbl_8047BA58(r2)
    stb r0, 0x67(r4)
    addi r3, r3, lbl_803A654C@l
    lfs f0, 0x58(r5)
    lfs f4, 0x50(r3)
    lfs f2, lbl_8047A494(r13)
    fadds f3, f0, f4
    lfs f0, 0x4c(r3)
    fmadds f0, f2, f0, f4
    fcmpo cr0, f3, f1
    stfs f3, 0x58(r5)
    stfs f0, 0x50(r3)
    ble @L_80037788
    lfs f0, 0x54(r3)
    stfs f1, 0x58(r5)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r4)
    @L_80037788:
    lis r3, lbl_803A654C@ha
    lfs f0, 0x58(r5)
    addi r3, r3, lbl_803A654C@l
    lfs f1, 0x54(r3)
    fadds f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r4)
    @L_800377AC:
    addi r1, r1, 0x10
    blr
