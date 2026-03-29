    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    bl fn_801D23C0
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, lbl_8047B998(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047B9CC(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_8010206C
    li r0, 0x1
    la r5, lbl_8047A3FC(r13)
    stw r31, lbl_8047A3FC(r13)
    li r3, 0x1
    li r4, 0x2
    stw r0, 0x4(r5)
    bl fn_8019075C
    li r3, 0x38f
    bl fn_800FF730
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f2, lbl_8047B998(r2)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047B9CC(r2)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_80102038
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
