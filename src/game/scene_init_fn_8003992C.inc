    nofralloc
    stwu r1, -0x10(r1)
    lwz r3, lbl_8047A4AC(r13)
    lwz r0, lbl_8047A4BC(r13)
    mulli r3, r3, 0x1f
    cmpwi r0, 0x0
    addi r3, r3, 0x9a
    bne @L_8003995C
    lfs f0, lbl_8047A4C0(r13)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r3, r3, r0
    @L_8003995C:
    extsh r0, r3
    li r3, 0x0
    sth r0, 0x52(r4)
    addi r1, r1, 0x10
    blr
