    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x47e(r6)
    cmplwi r0, 0x0
    bne @L_800D5F00
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800D5F00
    extsh r5, r3
    extsh r6, r4
    extsh r7, r7
    li r3, 0x12
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5F24
    @L_800D5F00:
    lis r5, fn_800D7444@ha
    addi r0, r5, fn_800D7444@l
    stw r0, 0x4c4(r6)
    lwz r5, lbl_8047AA80(r13)
    sth r3, 0x4cc(r5)
    lwz r3, lbl_8047AA80(r13)
    sth r4, 0x4ce(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r7, 0x4d0(r3)
    @L_800D5F24:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
