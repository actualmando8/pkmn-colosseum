    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r10, r5
    stw r0, 0x14(r1)
    lwz r8, lbl_8047AA80(r13)
    lwz r0, 0x0(r8)
    mr r8, r6
    cmpwi r0, 0x1
    bne @L_800DBD30
    mr r5, r3
    mr r6, r4
    mr r9, r7
    clrlwi r7, r10, 24
    clrlwi r8, r8, 24
    li r3, 0x53
    li r4, 0x5
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBD60
    @L_800DBD30:
    lis r5, lbl_80400B28@ha
    li r6, 0x1
    addi r5, r5, lbl_80400B28@l
    mulli r0, r3, 0x14
    add r3, r5, r3
    stb r6, 0x1fc(r3)
    add r3, r5, r0
    stbu r6, 0x20c(r3)
    stb r4, 0x1(r3)
    stb r10, 0xa(r3)
    stb r8, 0xb(r3)
    stb r7, 0x2(r3)
    @L_800DBD60:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
