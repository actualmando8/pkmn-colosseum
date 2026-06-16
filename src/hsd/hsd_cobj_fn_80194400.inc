    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x30(r1)
    beq @L_8019449C
    lha r0, 0x0(r4)
    lis r7, 0x4330
    lis r6, 0x4330
    lis r5, 0x4330
    xoris r8, r0, 0x8000
    lis r0, 0x4330
    stw r8, 0xc(r1)
    lfd f1, -23856(r2)
    stw r7, 0x8(r1)
    lfd f3, -23856(r2)
    lfd f0, 0x8(r1)
    stw r6, 0x10(r1)
    fsubs f0, f0, f1
    lfd f2, -23856(r2)
    stw r5, 0x18(r1)
    lfd f1, -23856(r2)
    stfs f0, 0xc(r3)
    lha r5, 0x2(r4)
    stw r0, 0x20(r1)
    xoris r0, r5, 0x8000
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f3
    stfs f0, 0x10(r3)
    lha r0, 0x4(r4)
    xoris r0, r0, 0x8000
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f2
    stfs f0, 0x14(r3)
    lha r0, 0x6(r4)
    xoris r0, r0, 0x8000
    stw r0, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x18(r3)
@L_8019449C:
    addi r1, r1, 0x30
    blr
