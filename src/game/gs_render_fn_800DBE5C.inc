    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DBE90
    mr r5, r3
    li r3, 0x51
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBEA4
    @L_800DBE90:
    lis r4, lbl_80400B28@ha
    li r5, 0x0
    addi r0, r4, lbl_80400B28@l
    add r3, r0, r3
    stb r5, 0x1fc(r3)
    @L_800DBEA4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
