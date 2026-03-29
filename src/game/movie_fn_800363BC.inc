    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lis r3, lbl_803A3E58@ha
    lfs f1, lbl_8047BA30(r2)
    addi r31, r3, lbl_803A3E58@l
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, lbl_8047A460(r13)
    li r7, 0x3
    addi r6, r31, 0x1640
    stw r7, 0x1318(r31)
    addi r9, r3, 0x1
    addi r5, r31, 0x1318
    li r0, 0x0
    li r8, 0x7
    lis r3, fn_800370E0@ha
    stw r9, lbl_8047A460(r13)
    addi r4, r3, fn_800370E0@l
    addi r6, r6, 0xffc
    stw r8, 0x4(r5)
    addi r3, r31, 0x1328
    li r7, 0x1000
    li r8, 0x10
    stw r0, 0x8(r5)
    li r9, 0x1
    stw r0, 0xc(r5)
    bl fn_800A19CC
    addi r3, r31, 0x1328
    bl fn_800A1F94
    b @L_80036448
    @L_80036444:
    bl fn_800F0308
    @L_80036448:
    lwz r0, lbl_8047A460(r13)
    cmpwi r0, 0x0
    bne @L_80036444
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
