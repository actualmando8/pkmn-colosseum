    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    lis r4, lbl_80266770@ha
    li r10, 0x0
    addi r9, r4, lbl_80266770@l
    lwz r8, 0x0(r9)
    lwz r7, 0x4(r9)
    lwz r6, 0x8(r9)
    lwz r5, 0xc(r9)
    lwz r4, 0x10(r9)
    lwz r0, 0x14(r9)
    stw r8, 0x8(r1)
    stw r7, 0xc(r1)
    stw r6, 0x10(r1)
    stw r5, 0x14(r1)
    stw r4, 0x18(r1)
    stw r0, 0x1c(r1)
    cmpw r3, r8
    beq @L_8000CBE4
    li r10, 0x1
    cmpw r3, r6
    beq @L_8000CBE4
    li r10, 0x2
    cmpw r3, r4
    beq @L_8000CBE4
    li r10, 0x3
    @L_8000CBE4:
    slwi r0, r10, 3
    addi r3, r1, 0xc
    lwzx r31, r3, r0
    bl fn_800FF560
    lis r5, fn_8000CB54@ha
    mr r4, r3
    addi r8, r5, fn_8000CB54@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    mr r5, r31
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800F0654
    li r3, 0x0
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
