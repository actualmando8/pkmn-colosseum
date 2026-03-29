    nofralloc
    stwu r1, -0x20(r1)
    lbz r0, 0x95(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80039020
    cmpwi r0, 0x8
    blt @L_80039024
    @L_80039020:
    li r0, 0x0
    @L_80039024:
    mulli r0, r0, 0xc
    lis r5, lbl_803A65B0@ha
    li r3, 0x0
    addi r5, r5, lbl_803A65B0@l
    lfsx f0, r5, r0
    add r5, r5, r0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r4)
    lfs f0, 0x4(r5)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r4)
    lfs f0, 0x8(r5)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x67(r4)
    addi r1, r1, 0x20
    blr
