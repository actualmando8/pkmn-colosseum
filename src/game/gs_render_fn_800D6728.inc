    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x47e(r3)
    cmplwi r0, 0x1
    bne @L_800D674C
    bl fn_800D6B00
    b @L_800D67AC
    @L_800D674C:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D676C
    li r3, 0x3
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D67AC
    @L_800D676C:
    bl fn_800D6B00
    lwz r4, lbl_8047AA80(r13)
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800D67AC
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800D67AC
    lwz r3, 0x24(r4)
    lwz r0, 0x20(r4)
    cmplw r3, r0
    bne @L_800D67AC
    li r0, 0x0
    stw r0, 0x24(r4)
    @L_800D67AC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
