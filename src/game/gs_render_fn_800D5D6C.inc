    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lbz r0, 0x47e(r5)
    cmplwi r0, 0x0
    bne @L_800D5DAC
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D5DAC
    clrlwi r5, r3, 24
    li r3, 0x15
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5DC0
    @L_800D5DAC:
    lis r4, fn_800D73F8@ha
    addi r0, r4, fn_800D73F8@l
    stw r0, 0x4c4(r5)
    lwz r4, lbl_8047AA80(r13)
    stb r3, 0x4c8(r4)
    @L_800D5DC0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
