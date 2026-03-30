    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D9FE8
    mr r5, r3
    li r3, 0x33
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA018
    @L_800D9FE8:
    cmpwi r3, 0x1
    bne @L_800D9FFC
    li r0, 0x0
    stb r0, 0x42c(r4)
    b @L_800DA00C
    @L_800D9FFC:
    cmpwi r3, 0x0
    bne @L_800DA00C
    li r0, 0x1
    stb r0, 0x42c(r4)
    @L_800DA00C:
    lwz r3, lbl_8047AA80(r13)
    lbz r3, 0x42c(r3)
    bl fn_800BD870
    @L_800DA018:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
