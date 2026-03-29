    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r4
    lwz r3, lbl_8047A390(r13)
    cmplwi r3, 0x0
    beq @L_80024D24
    lha r3, 0x2(r3)
    addi r0, r3, 0x8
    extsh r0, r0
    sth r0, 0x50(r30)
    lwz r3, lbl_8047A390(r13)
    lha r3, 0x4(r3)
    addi r0, r3, 0x8
    extsh r0, r0
    sth r0, 0x52(r30)
    @L_80024D24:
    lbz r31, 0x67(r30)
    bl fn_800D3088
    lis r4, 0x4330
    xoris r0, r31, 0x8000
    stw r3, 0xc(r1)
    lfd f2, lbl_8047B8D0(r2)
    stw r4, 0x8(r1)
    lfd f1, lbl_8047B8B8(r2)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f2, f0, f2
    lfs f3, lbl_8047A37C(r13)
    stw r4, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fmadds f0, f3, f2, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r31, 0x1c(r1)
    cmpwi r31, 0x40
    bge @L_80024D88
    fneg f0, f3
    li r31, 0x40
    stfs f0, lbl_8047A37C(r13)
    b @L_80024D9C
    @L_80024D88:
    cmpwi r31, 0xff
    ble @L_80024D9C
    fneg f0, f3
    li r31, 0xff
    stfs f0, lbl_8047A37C(r13)
    @L_80024D9C:
    clrlwi r0, r31, 24
    stb r0, 0x67(r30)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
