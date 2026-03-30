    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800DA500
    mr r5, r3
    mr r6, r4
    li r3, 0x2b
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA568
    @L_800DA500:
    cmpwi r3, 0x0
    bne @L_800DA52C
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    li r6, 0x5
    bl fn_800BCDDC
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x10
    stw r0, 0x8(r3)
    b @L_800DA568
    @L_800DA52C:
    lis r6, lbl_8031459C@ha
    lis r5, lbl_803145A8@ha
    slwi r8, r3, 2
    slwi r4, r4, 2
    addi r3, r6, lbl_8031459C@l
    addi r5, r5, lbl_803145A8@l
    slwi r0, r7, 2
    lwzx r3, r3, r8
    lwzx r4, r5, r4
    li r6, 0x5
    lwzx r5, r5, r0
    bl fn_800BCDDC
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x2000
    stw r0, 0x8(r3)
    @L_800DA568:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
