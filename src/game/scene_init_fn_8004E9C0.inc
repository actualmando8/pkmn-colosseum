    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    lwz r0, lbl_804788E8(r13)
    cmpwi r0, 0x0
    beq @L_8004EA0C
    lis r3, lbl_802EF0A8@ha
    li r0, 0x0
    addi r3, r3, lbl_802EF0A8@l
    stw r0, lbl_804788E8(r13)
    addis r4, r3, 0x1
    lha r3, -0x733a(r4)
    lha r0, -0x7356(r4)
    stw r3, lbl_8047A534(r13)
    stw r0, lbl_8047A530(r13)
    @L_8004EA0C:
    li r0, 0x0
    mr r3, r29
    stw r0, lbl_8047A538(r13)
    bl fn_801D16F0
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_8004EAC0
    mr r31, r30
    bl fn_8017B3E4
    lwz r4, lbl_8047A534(r13)
    la r5, lbl_8047A538(r13)
    lwz r3, lbl_8047A530(r13)
    la r0, lbl_8047A53C(r13)
    lfs f0, lbl_8047BE48(r2)
    stw r29, 0xc(r1)
    stw r5, 0x10(r1)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stfs f0, lbl_8047A53C(r13)
    stw r0, 0x8(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x77
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    b @L_8004EA8C
    @L_8004EA88:
    bl fn_800F0308
    @L_8004EA8C:
    mr r3, r31
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_8004EA88
    li r3, 0x77
    bl fn_80102510
    li r3, 0x77
    li r4, 0x1
    bl fn_80102428
    mr r3, r31
    bl fn_8017B1CC
    mr r3, r30
    bl fn_800F915C
    @L_8004EAC0:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
