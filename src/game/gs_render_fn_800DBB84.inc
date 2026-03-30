    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800DBBC0
    mr r5, r3
    mr r6, r4
    li r3, 0x55
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBBEC
    @L_800DBBC0:
    lis r5, lbl_80400B28@ha
    li r8, 0x1
    addi r6, r5, lbl_80400B28@l
    li r0, 0x3
    mulli r5, r3, 0x14
    add r3, r6, r3
    stb r8, 0x1fc(r3)
    add r3, r6, r5
    stbu r0, 0x20c(r3)
    stb r4, 0x1(r3)
    stb r7, 0x2(r3)
    @L_800DBBEC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
