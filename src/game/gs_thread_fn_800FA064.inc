    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    lha r0, 0x18(r31)
    cmpwi r0, 0x0
    beq @L_800FA14C
    lwz r3, 0x1c(r31)
    bl fn_800FA444
    lbz r0, 0x4a(r31)
    srwi r3, r3, 16
    extsh r5, r3
    cmpwi r0, 0x1
    beq @L_800FA0C8
    bge @L_800FA0B0
    cmpwi r0, 0x0
    bge @L_800FA0BC
    b @L_800FA14C
@L_800FA0B0:
    cmpwi r0, 0x3
    bge @L_800FA14C
    b @L_800FA108
@L_800FA0BC:
    lfs f0, 0x4(r31)
    stfs f0, 0xc(r31)
    b @L_800FA14C
@L_800FA0C8:
    lha r3, 0x18(r31)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    subf r3, r5, r3
    lfd f1, lbl_8047CD10(r2)
    srwi r0, r3, 31
    lfs f2, 0x4(r31)
    add r0, r0, r3
    srawi r0, r0, 1
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fadds f0, f2, f0
    stfs f0, 0xc(r31)
    b @L_800FA14C
@L_800FA108:
    lha r4, 0x18(r31)
    lis r3, 0x4330
    xoris r0, r5, 0x8000
    stw r3, 0x8(r1)
    xoris r4, r4, 0x8000
    lfd f2, lbl_8047CD10(r2)
    stw r4, 0xc(r1)
    lfs f3, 0x4(r31)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    fadds f1, f3, f1
    fsubs f0, f0, f2
    fsubs f0, f1, f0
    stfs f0, 0xc(r31)
@L_800FA14C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
