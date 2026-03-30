    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x1
    bne @L_800D6A24
    stw r3, 0x488(r4)
    b @L_800D6A4C
    @L_800D6A24:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D6A48
    mr r5, r3
    li r3, 0x1
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D6A4C
    @L_800D6A48:
    stw r3, 0x14(r4)
    @L_800D6A4C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
