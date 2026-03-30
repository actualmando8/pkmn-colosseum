    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D7854
    mr r5, r3
    li r3, 0x46
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D7858
    @L_800D7854:
    stw r3, 0x24(r4)
    @L_800D7858:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
