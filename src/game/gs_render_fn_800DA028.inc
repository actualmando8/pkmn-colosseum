    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800DA05C
    mr r5, r3
    li r3, 0x32
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA07C
    @L_800DA05C:
    lis r4, lbl_8031453C@ha
    slwi r0, r3, 2
    addi r3, r4, lbl_8031453C@l
    lwzx r0, r3, r0
    stw r0, 0x428(r5)
    lwz r3, lbl_8047AA80(r13)
    lwz r3, 0x428(r3)
    bl fn_800B94F0
    @L_800DA07C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
