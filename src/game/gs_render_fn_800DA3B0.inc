    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800DA3E8
    mr r5, r3
    clrlwi r6, r4, 24
    li r3, 0x2d
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA418
    @L_800DA3E8:
    cmpwi r3, 0x1
    bne @L_800DA3FC
    li r0, 0x1
    stb r0, 0x418(r5)
    b @L_800DA40C
    @L_800DA3FC:
    cmpwi r3, 0x0
    bne @L_800DA40C
    li r0, 0x0
    stb r0, 0x418(r5)
    @L_800DA40C:
    lwz r3, lbl_8047AA80(r13)
    lbz r3, 0x418(r3)
    bl fn_800BD008
    @L_800DA418:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
