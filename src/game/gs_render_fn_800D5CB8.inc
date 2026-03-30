    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r11, r5
    mr r10, r6
    stw r0, 0x14(r1)
    mr r9, r7
    lwz r8, lbl_8047AA80(r13)
    lbz r0, 0x47e(r8)
    cmplwi r0, 0x0
    bne @L_800D5D14
    lwz r0, 0x0(r8)
    cmpwi r0, 0x1
    bne @L_800D5D14
    mr r5, r3
    clrlwi r6, r4, 24
    clrlwi r7, r11, 24
    clrlwi r8, r10, 24
    clrlwi r9, r9, 24
    li r3, 0x16
    li r4, 0x5
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5D5C
    @L_800D5D14:
    slwi r0, r3, 2
    lis r5, fn_800D73C4@ha
    addi r6, r5, fn_800D73C4@l
    add r5, r8, r0
    stw r6, 0x4e0(r5)
    mulli r5, r3, 0xc
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r4, 0x4e8(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r11, 0x4e9(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r10, 0x4ea(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r9, 0x4eb(r3)
    @L_800D5D5C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
