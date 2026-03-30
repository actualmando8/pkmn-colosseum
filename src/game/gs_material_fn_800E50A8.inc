    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r31, 0x18
    stw r30, 0x8(r1)
    mr r30, r4
    bl fn_800E01D0
    lwz r31, 0x8(r31)
    cmplwi r31, 0x0
    bne @L_800E50E8
    la r3, lbl_8047CB60(r2)
    li r4, 0x3a9
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E50E8:
    cmplwi r30, 0x0
    bne @L_800E5104
    lis r4, lbl_80270E50@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E50@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E5104:
    lwz r3, 0x0(r30)
    lwz r0, 0x4(r30)
    stw r3, 0x38(r31)
    stw r0, 0x3c(r31)
    lwz r0, 0x8(r30)
    stw r0, 0x40(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E5170
    cmplwi r31, 0x0
    beq @L_800E5170
    bne @L_800E5144
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E5144:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E5160
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E5160
    li r3, 0x1
    @L_800E5160:
    cmpwi r3, 0x0
    bne @L_800E5170
    mr r3, r31
    bl fn_8019D620
    @L_800E5170:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
