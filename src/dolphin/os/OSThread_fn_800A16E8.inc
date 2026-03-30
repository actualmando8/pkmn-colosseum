    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r4
    @L_800A16FC:
    lwz r0, 0x2cc(r3)
    cmpwi r0, 0x0
    bgt @L_800A1724
    lwz r0, 0x2d0(r3)
    cmpw r0, r31
    ble @L_800A1724
    mr r4, r31
    bl fn_800A1528
    cmplwi r3, 0x0
    bne @L_800A16FC
    @L_800A1724:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
