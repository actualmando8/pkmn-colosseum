    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    stw r4, lbl_8047A568(r13)
    addi r29, r1, 0x8
    li r28, 0x0
    lis r3, lbl_8026768C@ha
    addi r31, r3, lbl_8026768C@l
    @L_80056B9C:
    lwz r4, 0x0(r31)
    cmpwi r28, 0x0
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r31)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    blt @L_80056BC4
    cmpwi r28, 0x3
    blt @L_80056BCC
    @L_80056BC4:
    li r30, -0x1
    b @L_80056BD0
    @L_80056BCC:
    lwz r30, 0x0(r29)
    @L_80056BD0:
    cmpwi r30, 0x0
    blt @L_80056BF8
    mr r3, r30
    li r4, 0x1f
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    @L_80056BF8:
    cmpw r27, r28
    beq @L_80056C0C
    mr r3, r30
    li r4, 0x0
    bl fn_80102254
    @L_80056C0C:
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    cmpwi r28, 0x3
    blt @L_80056B9C
    lfs f1, lbl_8047BEC0(r2)
    li r3, 0x1
    lfs f0, lbl_8047BEB4(r2)
    stw r27, lbl_8047A584(r13)
    stw r27, lbl_8047A580(r13)
    stfs f1, lbl_8047A57C(r13)
    stfs f1, lbl_8047A578(r13)
    stfs f1, lbl_8047A574(r13)
    stfs f0, lbl_8047A570(r13)
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
