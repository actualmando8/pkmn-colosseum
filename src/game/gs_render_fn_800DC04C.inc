    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r12, r5
    mr r11, r6
    stw r0, 0x14(r1)
    mr r10, r8
    lwz r9, lbl_8047AA80(r13)
    lwz r0, 0x0(r9)
    mr r9, r7
    cmpwi r0, 0x1
    bne @L_800DC0A0
    mr r5, r3
    mr r6, r4
    mr r7, r12
    mr r8, r11
    clrlwi r9, r9, 24
    li r3, 0x4c
    li r4, 0x6
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DC0C4
    @L_800DC0A0:
    mulli r5, r3, 0x5
    lis r3, lbl_80400B28@ha
    addi r0, r3, lbl_80400B28@l
    add r3, r0, r5
    stbu r4, 0x9b(r3)
    stb r12, 0x1(r3)
    stb r11, 0x2(r3)
    stb r9, 0x3(r3)
    stb r10, 0x4(r3)
    @L_800DC0C4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
