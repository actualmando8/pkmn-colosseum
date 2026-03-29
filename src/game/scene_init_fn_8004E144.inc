    nofralloc
    lfs f2, 0x70(r4)
    lfd f0, lbl_8047BE28(r2)
    lfd f1, lbl_8047BE30(r2)
    fadd f0, f2, f0
    frsp f0, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x70(r4)
    cror eq, gt, eq
    bne @L_8004E178
    lfs f0, 0x70(r4)
    fsub f0, f0, f1
    frsp f0, f0
    stfs f0, 0x70(r4)
    @L_8004E178:
    li r3, 0x0
    blr
