    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r8, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x47e(r6)
    cmplwi r0, 0x0
    bne @L_800D58EC
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800D58EC
    mr r5, r3
    extsh r6, r4
    extsh r7, r8
    li r3, 0x1e
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D591C
    @L_800D58EC:
    lis r5, fn_800D72C4@ha
    slwi r0, r3, 2
    slwi r7, r3, 4
    addi r5, r5, fn_800D72C4@l
    add r3, r6, r0
    stw r5, 0x500(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r7
    sth r4, 0x522(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r7
    sth r8, 0x524(r3)
    @L_800D591C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
