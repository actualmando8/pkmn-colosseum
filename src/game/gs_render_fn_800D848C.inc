    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    mr r8, r6
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r7, lbl_8047AA80(r13)
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D84E4
    mr r5, r29
    mr r6, r30
    mr r7, r31
    li r3, 0x27
    li r4, 0x13
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D85B8
    @L_800D84E4:
    cmpwi r30, 0x0
    bne @L_800D8524
    lis r4, lbl_80314404@ha
    lis r3, lbl_80314454@ha
    slwi r6, r29, 2
    slwi r0, r31, 2
    addi r4, r4, lbl_80314404@l
    addi r5, r3, lbl_80314454@l
    lwzx r3, r4, r6
    li r4, 0x1
    lwzx r5, r5, r0
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    bl fn_800B857C
    b @L_800D85B8
    @L_800D8524:
    cmpwi r30, 0x1
    bne @L_800D854C
    lis r3, lbl_803144A8@ha
    slwi r0, r29, 2
    addi r4, r3, lbl_803144A8@l
    mr r3, r8
    lwzx r4, r4, r0
    li r5, 0x0
    bl fn_800BD58C
    b @L_800D8570
    @L_800D854C:
    cmpwi r30, 0x2
    bne @L_800D8570
    lis r3, lbl_803144A8@ha
    slwi r0, r29, 2
    addi r4, r3, lbl_803144A8@l
    mr r3, r8
    lwzx r4, r4, r0
    li r5, 0x1
    bl fn_800BD58C
    @L_800D8570:
    lis r6, lbl_80314404@ha
    lis r5, lbl_80314424@ha
    lis r4, lbl_80314454@ha
    lis r3, lbl_803144A8@ha
    slwi r10, r29, 2
    addi r9, r6, lbl_80314404@l
    addi r6, r3, lbl_803144A8@l
    slwi r8, r30, 2
    addi r7, r5, lbl_80314424@l
    slwi r0, r31, 2
    addi r5, r4, lbl_80314454@l
    lwzx r3, r9, r10
    lwzx r4, r7, r8
    li r7, 0x0
    lwzx r5, r5, r0
    li r8, 0x7d
    lwzx r6, r6, r10
    bl fn_800B857C
    @L_800D85B8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
