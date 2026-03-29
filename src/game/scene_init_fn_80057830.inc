    nofralloc
    stwu r1, -0x40(r1)
    cmpwi r5, 0x0
    beq @L_800578E0
    lis r5, 0x4330
    xoris r6, r3, 0x8000
    xoris r0, r4, 0x8000
    stw r6, 0xc(r1)
    lis r3, lbl_803A9768@ha
    lfd f8, lbl_8047BEE8(r2)
    stw r5, 0x8(r1)
    addi r3, r3, lbl_803A9768@l
    lfs f1, lbl_8047BEF4(r2)
    lfd f2, 0x8(r1)
    lfs f0, lbl_8047BF00(r2)
    fsubs f7, f2, f8
    stw r0, 0x14(r1)
    stw r5, 0x10(r1)
    lfd f2, 0x10(r1)
    stw r6, 0x1c(r1)
    fsubs f6, f2, f8
    stw r5, 0x18(r1)
    lfd f2, 0x18(r1)
    stw r0, 0x24(r1)
    fsubs f5, f2, f8
    stw r5, 0x20(r1)
    lfd f2, 0x20(r1)
    stw r6, 0x2c(r1)
    fsubs f4, f2, f8
    stw r5, 0x28(r1)
    lfd f2, 0x28(r1)
    stw r0, 0x34(r1)
    fsubs f3, f2, f8
    stw r5, 0x30(r1)
    lfd f2, 0x30(r1)
    stfs f7, 0x27c(r3)
    fsubs f2, f2, f8
    stfs f6, 0x280(r3)
    stfs f5, 0x294(r3)
    stfs f4, 0x298(r3)
    stfs f3, 0x28c(r3)
    stfs f2, 0x290(r3)
    stfs f1, 0x284(r3)
    stfs f0, 0x288(r3)
    b @L_80057940
    @L_800578E0:
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    xoris r0, r4, 0x8000
    stw r3, 0x34(r1)
    lis r3, lbl_803A9768@ha
    lfd f4, lbl_8047BEE8(r2)
    stw r5, 0x30(r1)
    addi r3, r3, lbl_803A9768@l
    lfs f6, 0x27c(r3)
    lfd f0, 0x30(r1)
    lfs f5, 0x280(r3)
    fsubs f3, f0, f4
    lfs f1, lbl_8047BF00(r2)
    lfs f0, lbl_8047BF08(r2)
    stw r0, 0x2c(r1)
    stw r5, 0x28(r1)
    lfd f2, 0x28(r1)
    stfs f6, 0x28c(r3)
    fsubs f2, f2, f4
    stfs f5, 0x290(r3)
    stfs f3, 0x294(r3)
    stfs f2, 0x298(r3)
    stfs f1, 0x284(r3)
    stfs f0, 0x288(r3)
    @L_80057940:
    addi r1, r1, 0x40
    blr
