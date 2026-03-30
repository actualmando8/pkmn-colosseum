    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r10, r5
    mr r9, r7
    stw r0, 0x14(r1)
    lwz r8, lbl_8047AA80(r13)
    lwz r0, 0x0(r8)
    mr r8, r6
    cmpwi r0, 0x1
    bne @L_800DC01C
    mr r5, r3
    mr r6, r4
    mr r7, r10
    li r3, 0x4d
    li r4, 0x5
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DC03C
    @L_800DC01C:
    lis r5, lbl_80400B28@ha
    slwi r3, r3, 2
    addi r0, r5, lbl_80400B28@l
    add r3, r0, r3
    stbu r4, 0x12b(r3)
    stb r10, 0x1(r3)
    stb r8, 0x2(r3)
    stb r9, 0x3(r3)
    @L_800DC03C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
