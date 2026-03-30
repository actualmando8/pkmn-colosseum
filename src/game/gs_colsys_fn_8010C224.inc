    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lhz r0, lbl_8047AD50(r13)
    stw r3, lbl_8047AD48(r13)
    cmplwi r0, 0x0
    bne @L_8010C264
    rlwinm r3, r3, 4, 0, 27
    bl fn_800E3534
    sth r3, lbl_8047AD50(r13)
    rlwinm r3, r3, 0, 16, 31
    bl fn_800E27B0
    stw r3, lbl_8047AD4C(r13)
@L_8010C264:
    lwz r0, lbl_8047AD48(r13)
    li r4, 0x0
    lwz r3, lbl_8047AD4C(r13)
    rlwinm r5, r0, 4, 0, 27
    bl memset
    lhz r0, lbl_8047AD58(r13)
    cmplwi r0, 0x0
    bne @L_8010C2A0
    lwz r0, lbl_8047AD48(r13)
    rlwinm r3, r0, 3, 0, 28
    bl fn_800E3534
    sth r3, lbl_8047AD58(r13)
    rlwinm r3, r3, 0, 16, 31
    bl fn_800E27B0
    stw r3, lbl_8047AD54(r13)
@L_8010C2A0:
    lwz r0, lbl_8047AD48(r13)
    li r4, 0x0
    lwz r3, lbl_8047AD54(r13)
    rlwinm r5, r0, 3, 0, 28
    bl memset
    li r31, 0x0
    li r29, 0x0
    mr r30, r31
    b @L_8010C33C
@L_8010C2C4:
    lwz r3, lbl_8047AD54(r13)
    addi r0, r31, 0x2
    lhzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_8010C308
    li r3, 0x6ec0
    li r4, 0x20
    bl fn_800E2C04
    lwz r4, lbl_8047AD54(r13)
    addi r0, r31, 0x2
    sthx r3, r4, r0
    lwz r3, lbl_8047AD54(r13)
    lhzx r3, r3, r0
    bl fn_800E27B0
    lwz r4, lbl_8047AD54(r13)
    addi r0, r31, 0x4
    stwx r3, r4, r0
@L_8010C308:
    lwz r3, lbl_8047AD54(r13)
    addi r0, r31, 0x4
    li r4, 0x0
    li r5, 0x6ec0
    lwzx r3, r3, r0
    bl memset
    lwz r3, lbl_8047AD4C(r13)
    extsb r4, r29
    addi r0, r30, 0x7
    addi r31, r31, 0x8
    stbx r4, r3, r0
    addi r30, r30, 0x10
    addi r29, r29, 0x1
@L_8010C33C:
    lwz r0, lbl_8047AD48(r13)
    cmpw r29, r0
    blt @L_8010C2C4
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

