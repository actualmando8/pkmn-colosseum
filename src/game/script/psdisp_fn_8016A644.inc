    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r3, 0x50(r3)
    cmplwi r3, 0x0
    bne @L_8016A674
    li r3, -0x1
    b @L_8016A6E0
@L_8016A674:
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_8016A688
    li r0, 0x0
    stw r0, 0x4(r3)
@L_8016A688:
    lwz r4, 0x50(r29)
    lhz r3, 0x32(r4)
    subi r0, r3, 0x1
    rlwinm. r30, r0, 0, 16, 31
    sth r0, 0x32(r4)
    bne @L_8016A6D4
    lwz r31, 0x50(r29)
    lwz r12, 0x6c(r31)
    cmplwi r12, 0x0
    beq @L_8016A6BC
    mr r3, r31
    mtctr r12
    bctrl
@L_8016A6BC:
    lwz r0, -22268(r13)
    stw r0, 0x0(r31)
    lhz r3, -22282(r13)
    stw r31, -22268(r13)
    subi r0, r3, 0x1
    sth r0, -22282(r13)
@L_8016A6D4:
    li r0, 0x0
    mr r3, r30
    stw r0, 0x50(r29)
@L_8016A6E0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
