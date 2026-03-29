    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lbz r0, 0x95(r3)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    srwi r0, r3, 16
    sth r0, 0x8(r1)
    lbz r0, 0x8(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_8001669C
    lwz r0, lbl_8047A2D8(r13)
    cmpwi r0, -0x1
    bne @L_8001669C
    lfs f1, lbl_8047B744(r2)
    lfs f0, lbl_8047A2C4(r13)
    lfs f2, lbl_8047B740(r2)
    fsubs f0, f1, f0
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x67(r31)
    b @L_800166A4
    @L_8001669C:
    li r0, 0x0
    stb r0, 0x67(r31)
    @L_800166A4:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
