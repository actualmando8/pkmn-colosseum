    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r5, lbl_8047AA80(r13)
    lbz r0, 0x47e(r5)
    cmplwi r0, 0x0
    bne @L_800D62D4
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D62D4
    clrlwi r5, r30, 16
    clrlwi r6, r31, 16
    li r3, 0xb
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D6304
    @L_800D62D4:
    bl fn_800D6B00
    lis r4, fn_800D7508@ha
    lwz r3, lbl_8047AA80(r13)
    addi r4, r4, fn_800D7508@l
    li r0, 0x1
    stw r4, 0x4a8(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r30, 0x4b0(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r31, 0x4b2(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x49f(r3)
    @L_800D6304:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
