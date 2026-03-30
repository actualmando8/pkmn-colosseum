    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800DBA2C
    mr r5, r3
    mr r6, r4
    li r3, 0x59
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBA44
    @L_800DBA2C:
    lis r5, lbl_80400B28@ha
    slwi r3, r3, 2
    addi r0, r5, lbl_80400B28@l
    add r3, r0, r3
    stbu r4, 0x34d(r3)
    stb r7, 0x1(r3)
    @L_800DBA44:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
