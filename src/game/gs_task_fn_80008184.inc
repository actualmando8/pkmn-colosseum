    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    lis r3, lbl_80266678@ha
    lbz r31, lbl_80478828(r13)
    addi r5, r3, lbl_80266678@l
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lhz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    sth r0, 0x10(r1)
    bl fn_80106160
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80008228
    addi r4, r1, 0x8
    lis r3, 0x51ec
    lbzx r4, r4, r31
    subi r5, r3, 0x7ae1
    lbz r0, lbl_8047A270(r13)
    mullw r3, r30, r4
    cmplwi r0, 0x1
    mulhwu r0, r5, r3
    srwi r30, r0, 5
    bne @L_80008228
    li r30, 0x0
    @L_800081FC:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 23, 23
    cmplwi r0, 0x0
    bne @L_80008228
    bl fn_800F0308
    b @L_800081FC
    @L_80008228:
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
