    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D9F0C
    mr r5, r3
    li r3, 0x35
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D9F30
    @L_800D9F0C:
    cmpwi r3, 0x1
    bne @L_800D9F20
    li r0, 0x1
    stb r0, 0x1a(r4)
    b @L_800D9F30
    @L_800D9F20:
    cmpwi r3, 0x0
    bne @L_800D9F30
    li r0, 0x0
    stb r0, 0x1a(r4)
    @L_800D9F30:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
