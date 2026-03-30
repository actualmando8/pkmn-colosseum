    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    mr r31, r30
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EE184
    addi r31, r31, 0x1
    @L_800EE184:
    li r0, 0x0
    stw r31, lbl_8047ABAC(r13)
    mr r3, r29
    stw r0, lbl_8047ABB0(r13)
    stw r0, lbl_8047ABA8(r13)
    bl fn_800E5188
    cmpwi r31, 0x0
    beq @L_800EE1C8
    lis r4, fn_800EE20C@ha
    li r5, 0x0
    addi r4, r4, fn_800EE20C@l
    bl fn_801A3918
    lwz r0, lbl_8047ABA8(r13)
    cmplwi r0, 0x0
    bne @L_800EE1CC
    li r3, 0x0
    b @L_800EE1F0
    @L_800EE1C8:
    stw r3, lbl_8047ABA8(r13)
    @L_800EE1CC:
    bl fn_800EE834
    cmplwi r3, 0x0
    bne @L_800EE1E0
    li r3, 0x0
    b @L_800EE1F0
    @L_800EE1E0:
    stw r29, 0x4(r3)
    lwz r0, lbl_8047ABA8(r13)
    stw r0, 0x8(r3)
    sth r30, 0x2(r3)
    @L_800EE1F0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
