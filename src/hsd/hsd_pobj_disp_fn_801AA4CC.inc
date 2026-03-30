    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, 0x0(r3)
    rlwinm. r0, r0, 25, 31, 31
    beq @L_801AA4FC
    lwz r4, 0x8(r3)
    lwz r0, 0x14(r3)
    cmplw r4, r0
    blt @L_801AA4FC
    li r3, 0x0
    b @L_801AA528
@L_801AA4FC:
    lwz r4, 0x8(r3)
    addi r0, r4, 0x1
    stw r0, 0x8(r3)
    lwz r4, 0x8(r3)
    lwz r0, 0x10(r3)
    cmplw r4, r0
    ble @L_801AA520
    lwz r0, 0x8(r3)
    stw r0, 0x10(r3)
@L_801AA520:
    lwz r3, 0x20(r3)
    bl fn_801A6928
@L_801AA528:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
