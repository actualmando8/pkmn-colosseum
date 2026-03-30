    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r6, 0x0
    lfs f1, lbl_8047CB7C(r2)
    stw r0, 0x54(r1)
    addi r4, r1, 0x8
    lfs f0, lbl_8047CB80(r2)
    stw r6, 0x8(r1)
    lwz r3, 0x4(r3)
    lwz r5, 0x0(r3)
    lwz r0, 0x4(r5)
    ori r0, r0, 0x1000
    stw r5, 0x10(r1)
    stw r0, 0xc(r1)
    stw r6, 0x14(r1)
    stw r6, 0x18(r1)
    stfs f1, 0x1c(r1)
    stfs f1, 0x20(r1)
    stfs f1, 0x24(r1)
    stfs f0, 0x28(r1)
    stfs f0, 0x2c(r1)
    stfs f0, 0x30(r1)
    stfs f1, 0x34(r1)
    stfs f1, 0x38(r1)
    stfs f1, 0x3c(r1)
    stw r6, 0x40(r1)
    stw r6, 0x44(r1)
    bl fn_800E51A4
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
