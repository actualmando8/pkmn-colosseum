    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r12, r5
    mr r11, r6
    stw r0, 0x14(r1)
    mr r10, r8
    lwz r9, lbl_8047AA80(r13)
    lwz r0, 0x0(r9)
    cmpwi r0, 0x1
    bne @L_800DA154
    mr r5, r3
    mr r6, r4
    mr r8, r11
    mr r9, r7
    clrlwi r7, r12, 24
    clrlwi r10, r10, 24
    li r3, 0x30
    li r4, 0x6
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA1D8
    @L_800DA154:
    cmpwi r3, 0x1
    bne @L_800DA168
    li r0, 0x1
    stb r0, 0x425(r9)
    b @L_800DA178
    @L_800DA168:
    cmpwi r3, 0x0
    bne @L_800DA178
    li r0, 0x0
    stb r0, 0x425(r9)
    @L_800DA178:
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x425(r3)
    cmplwi r0, 0x0
    bne @L_800DA1A4
    li r3, 0x7
    li r4, 0x0
    li r5, 0x1
    li r6, 0x7
    li r7, 0x0
    bl fn_800BC618
    b @L_800DA1D8
    @L_800DA1A4:
    lis r6, lbl_8031457C@ha
    lis r3, lbl_8031456C@ha
    slwi r8, r4, 2
    slwi r5, r11, 2
    addi r6, r6, lbl_8031457C@l
    addi r4, r3, lbl_8031456C@l
    slwi r0, r7, 2
    lwzx r3, r6, r8
    lwzx r5, r4, r5
    mr r4, r12
    lwzx r6, r6, r0
    mr r7, r10
    bl fn_800BC618
    @L_800DA1D8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
