    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0x8(r3)
    cmplwi r31, 0x0
    beq @L_800E4578
    bne @L_800E454C
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E454C:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4568
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4568
    li r3, 0x1
    @L_800E4568:
    cmpwi r3, 0x0
    beq @L_800E4578
    mr r3, r31
    bl fn_8019D9DC
    @L_800E4578:
    lwz r3, 0x8(r30)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r3, r3, 0x44
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
