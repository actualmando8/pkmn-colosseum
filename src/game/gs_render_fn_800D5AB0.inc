    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r7, lbl_8047AA80(r13)
    lbz r0, 0x47e(r7)
    cmplwi r0, 0x0
    bne @L_800D5AF4
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D5AF4
    mr r5, r3
    clrlwi r6, r4, 16
    li r3, 0x1a
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5B18
    @L_800D5AF4:
    slwi r0, r3, 2
    lis r5, fn_800D7344@ha
    addi r6, r5, fn_800D7344@l
    add r5, r7, r0
    stw r6, 0x4e0(r5)
    mulli r0, r3, 0xc
    lwz r3, lbl_8047AA80(r13)
    add r3, r3, r0
    sth r4, 0x4ec(r3)
    @L_800D5B18:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
