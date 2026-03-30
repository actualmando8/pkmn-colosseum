    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r27, 0x1c(r1)
    mr. r29, r4
    addi r28, r3, 0x0
    addi r30, r5, 0x0
    blt @L_800B44B0
    cmpwi r29, 0x7f
    blt @L_800B44B8
    @L_800B44B0:
    li r3, -0x80
    b @L_800B45D4
    @L_800B44B8:
    li r0, -0x1
    stw r0, 0x0(r30)
    addi r3, r28, 0x0
    addi r4, r1, 0x14
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B44D8
    b @L_800B45D4
    @L_800B44D8:
    lwz r3, 0x14(r1)
    bl fn_800B1E0C
    slwi r0, r29, 6
    lwz r27, 0x14(r1)
    add r3, r3, r0
    lbz r0, 0x0(r3)
    addi r31, r3, 0x0
    cmplwi r0, 0xff
    bne @L_800B4504
    li r4, -0x4
    b @L_800B4554
    @L_800B4504:
    lis r3, lbl_803FC840@ha
    lwz r4, 0x10c(r27)
    addi r0, r3, lbl_803FC840@l
    cmplw r4, r0
    beq @L_800B4548
    addi r3, r31, 0x0
    li r5, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B4550
    lwz r4, 0x10c(r27)
    addi r3, r31, 0x4
    li r5, 0x2
    addi r4, r4, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B4550
    @L_800B4548:
    li r4, 0x0
    b @L_800B4554
    @L_800B4550:
    li r4, -0xa
    @L_800B4554:
    cmpwi r4, -0xa
    bne @L_800B4588
    lbz r0, 0x0(r31)
    cmplwi r0, 0xff
    bne @L_800B4570
    li r4, -0x4
    b @L_800B4588
    @L_800B4570:
    lbz r0, 0x34(r31)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800B4584
    li r4, 0x0
    b @L_800B4588
    @L_800B4584:
    li r4, -0xa
    @L_800B4588:
    cmpwi r4, 0x0
    blt @L_800B45CC
    lhz r5, 0x36(r31)
    cmplwi r5, 0x5
    blt @L_800B45AC
    lwz r3, 0x14(r1)
    lhz r0, 0x10(r3)
    cmplw r5, r0
    blt @L_800B45B4
    @L_800B45AC:
    li r4, -0x6
    b @L_800B45CC
    @L_800B45B4:
    stw r28, 0x0(r30)
    li r0, 0x0
    stw r29, 0x4(r30)
    stw r0, 0x8(r30)
    lhz r0, 0x36(r31)
    sth r0, 0x10(r30)
    @L_800B45CC:
    lwz r3, 0x14(r1)
    bl fn_800B02F4
    @L_800B45D4:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
