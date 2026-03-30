    nofralloc
    stwu r1, -0x10(r1)
    lis r4, 0x3
    lis r0, 0x4330
    lfd f2, -22872(r2)
    lwz r5, lbl_80478C94(r13)
    addi r4, r4, 0x43fd
    lwz r3, lbl_80478C94(r13)
    lwz r5, 0x0(r5)
    stw r0, 0x8(r1)
    mullw r4, r5, r4
    lfs f0, -22880(r2)
    addis r4, r4, 0x27
    subi r0, r4, 0x613d
    stw r0, 0x0(r3)
    lwz r3, lbl_80478C94(r13)
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 16, 16, 31
    stw r0, 0xc(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f0
    addi r1, r1, 0x10
    blr
