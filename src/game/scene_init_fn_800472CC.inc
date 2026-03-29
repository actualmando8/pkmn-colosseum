    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lis r5, lbl_803A6818@ha
    lis r4, lbl_802E52C8@ha
    addi r5, r5, lbl_803A6818@l
    lfs f1, lbl_8047BCA0(r2)
    lfs f0, 0x4c(r5)
    addi r31, r4, lbl_802E52C8@l
    li r30, 0x0
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    @L_80047310:
    mr r3, r31
    bl fn_800473E0
    addi r31, r31, 0x24
    addi r30, r30, 0x1
    cmpwi r30, 0x8
    blt @L_80047310
    lwz r4, lbl_8047A4EC(r13)
    lis r3, lbl_802E52C8@ha
    addi r0, r3, lbl_802E52C8@l
    subi r4, r4, 0x1
    mulli r3, r4, 0x24
    add r3, r0, r3
    addi r0, r4, 0x1
    lfs f2, lbl_8047BCC4(r2)
    lfs f0, lbl_8047BC94(r2)
    mtctr r0
    cmpwi r4, 0x0
    blt @L_8004737C
    @L_80047358:
    lfs f1, 0x1c(r3)
    fsubs f1, f1, f2
    stfs f1, 0x1c(r3)
    lfs f1, 0x1c(r3)
    fcmpo cr0, f1, f0
    bge @L_80047374
    stfs f0, 0x1c(r3)
    @L_80047374:
    subi r3, r3, 0x24
    bdnz @L_80047358
    @L_8004737C:
    lis r3, lbl_802E52C8@ha
    lwz r4, lbl_8047A4EC(r13)
    addi r3, r3, lbl_802E52C8@l
    addi r3, r3, 0xfc
    subfic r0, r4, 0x7
    lfs f2, lbl_8047BCC4(r2)
    lfs f0, lbl_8047BC94(r2)
    mtctr r0
    cmplwi r4, 0x7
    bge @L_800473C8
    @L_800473A4:
    lfs f1, 0x1c(r3)
    fsubs f1, f1, f2
    stfs f1, 0x1c(r3)
    lfs f1, 0x1c(r3)
    fcmpo cr0, f1, f0
    bge @L_800473C0
    stfs f0, 0x1c(r3)
    @L_800473C0:
    subi r3, r3, 0x24
    bdnz @L_800473A4
    @L_800473C8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
