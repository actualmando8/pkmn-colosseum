    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D9E80
    mr r5, r3
    li r3, 0x36
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D9EC8
    @L_800D9E80:
    cmpwi r3, 0x1
    bne @L_800D9E94
    li r0, 0x1
    stb r0, 0x5d(r4)
    b @L_800D9EA4
    @L_800D9E94:
    cmpwi r3, 0x0
    bne @L_800D9EA4
    li r0, 0x0
    stb r0, 0x5d(r4)
    @L_800D9EA4:
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x5d(r3)
    cmplwi r0, 0x1
    bne @L_800D9EC0
    lwz r3, lbl_8047AA8C(r13)
    bl fn_8019BD18
    b @L_800D9EC8
    @L_800D9EC0:
    li r3, 0x0
    bl fn_8019BD18
    @L_800D9EC8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
