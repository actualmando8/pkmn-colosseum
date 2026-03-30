    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DA0C0
    mr r5, r3
    li r3, 0x31
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA0F0
    @L_800DA0C0:
    cmpwi r3, 0x1
    bne @L_800DA0D4
    li r0, 0x1
    stb r0, 0x426(r4)
    b @L_800DA0E4
    @L_800DA0D4:
    cmpwi r3, 0x0
    bne @L_800DA0E4
    li r0, 0x0
    stb r0, 0x426(r4)
    @L_800DA0E4:
    lwz r3, lbl_8047AA80(r13)
    lbz r3, 0x426(r3)
    bl fn_800BCFDC
    @L_800DA0F0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
