    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    lha r0, lbl_8047AC70(r13)
    lis r7, 0x4330
    lha r8, lbl_8047AC72(r13)
    add r3, r0, r3
    stw r7, 0x8(r1)
    xoris r0, r3, 0x8000
    add r4, r8, r4
    stw r0, 0xc(r1)
    add r5, r3, r5
    add r0, r4, r6
    xoris r4, r4, 0x8000
    xoris r3, r5, 0x8000
    lfd f3, lbl_8047CD50(r2)
    xoris r0, r0, 0x8000
    lfd f0, 0x8(r1)
    stw r4, 0x1c(r1)
    fsubs f1, f0, f3
    lfs f4, lbl_80478B10(r13)
    stw r7, 0x18(r1)
    lfs f5, lbl_80478B14(r13)
    lfd f0, 0x18(r1)
    fmuls f2, f1, f4
    stw r3, 0x2c(r1)
    fsubs f1, f0, f3
    stw r7, 0x28(r1)
    fctiwz f2, f2
    lfd f0, 0x28(r1)
    fmuls f1, f1, f5
    stfd f2, 0x10(r1)
    fsubs f0, f0, f3
    fctiwz f2, f1
    lwz r3, 0x14(r1)
    stw r0, 0x3c(r1)
    fmuls f1, f0, f4
    cmpwi r3, 0x280
    stw r7, 0x38(r1)
    lfd f0, 0x38(r1)
    fctiwz f1, f1
    stfd f2, 0x20(r1)
    fsubs f0, f0, f3
    stfd f1, 0x30(r1)
    lwz r4, 0x24(r1)
    fmuls f0, f0, f5
    lwz r5, 0x34(r1)
    fctiwz f0, f0
    stfd f0, 0x40(r1)
    lwz r0, 0x44(r1)
    blt @L_800FE45C
    li r3, 0x27f
@L_800FE45C:
    cmpwi r4, 0x1e0
    blt @L_800FE468
    li r4, 0x1df
@L_800FE468:
    cmpwi r5, 0x280
    blt @L_800FE474
    li r5, 0x27f
@L_800FE474:
    cmpwi r0, 0x1e0
    blt @L_800FE480
    li r0, 0x1df
@L_800FE480:
    cmpwi r3, 0x0
    bge @L_800FE48C
    li r3, 0x0
@L_800FE48C:
    cmpwi r4, 0x0
    bge @L_800FE498
    li r4, 0x0
@L_800FE498:
    cmpwi r5, 0x0
    bge @L_800FE4A4
    li r5, 0x0
@L_800FE4A4:
    cmpwi r0, 0x0
    bge @L_800FE4B0
    li r0, 0x0
@L_800FE4B0:
    clrlwi r3, r3, 16
    clrlwi r4, r4, 16
    clrlwi r5, r5, 16
    clrlwi r6, r0, 16
    bl fn_800D9D68
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
