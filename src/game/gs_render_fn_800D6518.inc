    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x47e(r6)
    cmplwi r0, 0x0
    bne @L_800D6578
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800D6578
    extsh r5, r29
    extsh r6, r30
    extsh r7, r31
    li r3, 0x7
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D65B0
    @L_800D6578:
    bl fn_800D6B00
    lis r4, fn_800D7588@ha
    lwz r3, lbl_8047AA80(r13)
    addi r4, r4, fn_800D7588@l
    li r0, 0x1
    stw r4, 0x4a8(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r29, 0x4b0(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r30, 0x4b2(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r31, 0x4b4(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x49f(r3)
    @L_800D65B0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
