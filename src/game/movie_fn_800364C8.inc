    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r10, 0x0
    lis r3, lbl_803A3E58@ha
    addi r31, r3, lbl_803A3E58@l
    stb r10, lbl_8047A468(r13)
    lwz r3, lbl_8047A460(r13)
    addi r5, r31, 0x1318
    li r7, 0x5
    li r8, 0x4
    addi r9, r3, 0x1
    li r0, 0x6
    lis r3, fn_800370E0@ha
    addi r6, r31, 0x1640
    stw r9, lbl_8047A460(r13)
    addi r4, r3, fn_800370E0@l
    addi r3, r31, 0x1328
    addi r6, r6, 0xffc
    stw r7, 0x1318(r31)
    li r7, 0x1000
    li r9, 0x1
    stw r8, 0x4(r5)
    li r8, 0x10
    stw r0, 0x8(r5)
    stw r10, 0xc(r5)
    bl fn_800A19CC
    addi r3, r31, 0x1328
    bl fn_800A1F94
    b @L_80036548
    @L_80036544:
    bl fn_800F0308
    @L_80036548:
    lwz r0, lbl_8047A460(r13)
    cmpwi r0, 0x0
    bne @L_80036544
    lfs f1, lbl_8047BA30(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, lbl_8026702C@ha
    li r4, 0x0
    addi r3, r3, lbl_8026702C@l
    bl fn_801E189C
    li r3, 0x4d1
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
    lis r3, 0x8000
    li r4, 0x0
    addi r3, r3, 0x1805
    li r5, 0x17fb
    bl memset
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
