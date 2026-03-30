    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D30DC
    li r3, 0x4
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D30E0
    @L_800D30DC:
    bl fn_800B8920
    @L_800D30E0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
