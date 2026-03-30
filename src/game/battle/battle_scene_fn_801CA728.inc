    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    bl fn_8006ADEC
    mr r31, r3
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    bl fn_8025D0A8
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, -21888(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f2
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r31, r31, r0
    mr r3, r31
    bl fn_8006ADB4
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
