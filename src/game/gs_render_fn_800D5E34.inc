    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x47e(r6)
    cmplwi r0, 0x0
    bne @L_800D5E80
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800D5E80
    extsb r5, r3
    extsb r6, r4
    extsb r7, r7
    li r3, 0x13
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5EA4
    @L_800D5E80:
    lis r5, fn_800D7420@ha
    addi r0, r5, fn_800D7420@l
    stw r0, 0x4c4(r6)
    lwz r5, lbl_8047AA80(r13)
    stb r3, 0x4c8(r5)
    lwz r3, lbl_8047AA80(r13)
    stb r4, 0x4c9(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r7, 0x4ca(r3)
    @L_800D5EA4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
