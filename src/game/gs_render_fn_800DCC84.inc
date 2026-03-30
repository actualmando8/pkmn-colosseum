    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    lfs f2, 0x0(r4)
    lfs f1, 0x4(r4)
    lfs f0, 0x8(r4)
    fctiwz f2, f2
    fctiwz f1, f1
    addi r4, r1, 0x8
    fctiwz f0, f0
    stfd f2, 0x10(r1)
    stfd f1, 0x18(r1)
    lwz r6, 0x14(r1)
    stfd f0, 0x20(r1)
    lwz r5, 0x1c(r1)
    lwz r0, 0x24(r1)
    stb r6, 0xc(r1)
    stb r5, 0xd(r1)
    stb r0, 0xe(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x8(r1)
    lwz r3, 0xc(r3)
    bl fn_801A4A48
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
