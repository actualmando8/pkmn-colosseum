    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r7, lbl_8047AA80(r13)
    lbz r0, 0x47e(r7)
    cmplwi r0, 0x0
    bne @L_800D57E0
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D57E0
    mr r5, r3
    clrlwi r6, r4, 16
    li r3, 0x21
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5804
    @L_800D57E0:
    slwi r5, r3, 2
    lis r6, fn_800D7268@ha
    addi r6, r6, fn_800D7268@l
    slwi r0, r3, 4
    add r3, r7, r5
    stw r6, 0x500(r3)
    lwz r3, lbl_8047AA80(r13)
    add r3, r3, r0
    sth r4, 0x522(r3)
    @L_800D5804:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
