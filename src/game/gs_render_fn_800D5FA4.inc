    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x0
    bne @L_800D5FEC
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D5FEC
    clrlwi r5, r31, 24
    li r3, 0x10
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D6014
    @L_800D5FEC:
    bl fn_800D6B00
    lis r4, fn_800D748C@ha
    lwz r3, lbl_8047AA80(r13)
    addi r4, r4, fn_800D748C@l
    li r0, 0x1
    stw r4, 0x4a8(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r31, 0x4ac(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x49f(r3)
    @L_800D6014:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
