    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r9, r5
    mr r8, r6
    stw r0, 0x14(r1)
    lwz r7, lbl_8047AA80(r13)
    lbz r0, 0x47e(r7)
    cmplwi r0, 0x0
    bne @L_800D5C6C
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D5C6C
    mr r5, r3
    clrlwi r6, r4, 24
    clrlwi r7, r9, 24
    clrlwi r8, r8, 24
    li r3, 0x17
    li r4, 0x4
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5CA8
    @L_800D5C6C:
    slwi r0, r3, 2
    lis r5, fn_800D7398@ha
    addi r6, r5, fn_800D7398@l
    add r5, r7, r0
    stw r6, 0x4e0(r5)
    mulli r5, r3, 0xc
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r4, 0x4e8(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r9, 0x4e9(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r5
    stb r8, 0x4ea(r3)
    @L_800D5CA8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
