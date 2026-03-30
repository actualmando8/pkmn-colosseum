    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DBAD8
    mr r5, r3
    li r3, 0x57
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBAFC
    @L_800DBAD8:
    lis r4, lbl_80400B28@ha
    li r6, 0x1
    addi r5, r4, lbl_80400B28@l
    li r4, 0x5
    mulli r0, r3, 0x14
    add r3, r5, r3
    stb r6, 0x1fc(r3)
    add r3, r5, r0
    stb r4, 0x20c(r3)
    @L_800DBAFC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
