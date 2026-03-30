    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r0, 0x0(r3)
    lwz r3, 0x8(r3)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E730C
    lwz r31, 0x10(r3)
    cmplwi r31, 0x0
    bne @L_800E72D4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3e4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E72D4:
    cmplwi r30, 0x0
    bne @L_800E72F0
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3e5
    bl fn_80196E10
    @L_800E72F0:
    lwz r3, 0x38(r31)
    lwz r0, 0x3c(r31)
    stw r3, 0x0(r30)
    stw r0, 0x4(r30)
    lwz r0, 0x40(r31)
    stw r0, 0x8(r30)
    b @L_800E7314
    @L_800E730C:
    mr r3, r30
    bl fn_800E0204
    @L_800E7314:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
