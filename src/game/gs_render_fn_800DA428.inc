    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DA45C
    mr r5, r3
    li r3, 0x2c
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA4B4
    @L_800DA45C:
    cmpwi r3, 0x0
    bne @L_800DA488
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    li r6, 0x5
    bl fn_800BCDDC
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x10
    stw r0, 0x8(r3)
    b @L_800DA4B4
    @L_800DA488:
    lis r4, lbl_803145D0@ha
    slwi r0, r3, 2
    addi r4, r4, lbl_803145D0@l
    li r3, 0x2
    lwzx r6, r4, r0
    li r4, 0x1
    li r5, 0x1
    bl fn_800BCDDC
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x2000
    stw r0, 0x8(r3)
    @L_800DA4B4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
