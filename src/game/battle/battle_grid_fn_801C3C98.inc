    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    lis r3, lbl_80466DE8@ha
    addi r0, r3, lbl_80466DE8@l
    mr r7, r0
    beq @L_801C3D50
    li r5, 0x0
    b @L_801C3D44
    @L_801C3CC4:
    li r6, 0x0
    b @L_801C3D30
    @L_801C3CCC:
    clrlslwi r3, r6, 16, 2
    clrlwi r4, r6, 16
    addi r0, r3, 0x4
    lwzx r0, r7, r0
    cmplw r0, r31
    bne @L_801C3D2C
    slwi r0, r4, 2
    lis r3, lbl_80466DE8@ha
    add r4, r7, r0
    li r0, 0x0
    stw r0, 0x4(r4)
    addi r6, r3, lbl_80466DE8@l
    mr r3, r31
    li r4, 0x0
    lhz r5, 0xc(r7)
    subi r0, r5, 0x1
    sth r0, 0xc(r7)
    lhz r5, 0x42(r6)
    subi r0, r5, 0x1
    sth r0, 0x42(r6)
    bl fn_801DA4E8
    li r0, 0x0
    stb r0, 0x76(r31)
    b @L_801C3D50
    @L_801C3D2C:
    addi r6, r6, 0x1
    @L_801C3D30:
    clrlwi r0, r6, 16
    cmplwi r0, 0x2
    blt @L_801C3CCC
    addi r5, r5, 0x1
    addi r7, r7, 0x10
    @L_801C3D44:
    clrlwi r0, r5, 16
    cmplwi r0, 0x4
    blt @L_801C3CC4
    @L_801C3D50:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
