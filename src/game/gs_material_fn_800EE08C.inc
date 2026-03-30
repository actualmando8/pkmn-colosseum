    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_800EE0BC
    la r3, lbl_8047CC90(r2)
    li r4, 0xab
    la r5, lbl_8047CC98(r2)
    bl fn_80196E10
    @L_800EE0BC:
    lfs f1, 0xc(r30)
    lfs f0, 0x0(r31)
    fcmpo cr0, f1, f0
    ble @L_800EE0D0
    stfs f1, 0x0(r31)
    @L_800EE0D0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
