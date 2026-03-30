    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DBA88
    clrlwi r5, r3, 24
    li r3, 0x58
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBA94
    @L_800DBA88:
    lis r4, lbl_80400B28@ha
    addi r4, r4, lbl_80400B28@l
    stb r3, 0x34c(r4)
    @L_800DBA94:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
