    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    lis r3, lbl_80466DE8@ha
    addi r0, r3, lbl_80466DE8@l
    mr r5, r0
    beq @L_801C3D98
    cmplwi r31, 0x0
    bne @L_801C3DAC
    @L_801C3D98:
    lis r3, lbl_80275808@ha
    addi r3, r3, lbl_80275808@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801C3E24
    @L_801C3DAC:
    li r6, 0x0
    b @L_801C3E18
    @L_801C3DB4:
    li r7, 0x0
    b @L_801C3E04
    @L_801C3DBC:
    clrlslwi r3, r7, 16, 2
    clrlwi r4, r7, 16
    addi r0, r3, 0x4
    lwzx r0, r5, r0
    cmplw r0, r30
    bne @L_801C3E00
    slwi r0, r4, 2
    mr r3, r30
    add r5, r5, r0
    li r4, 0x0
    stw r31, 0x4(r5)
    bl fn_801DA4E8
    lbz r3, 0x76(r30)
    li r0, 0x0
    stb r3, 0x76(r31)
    stb r0, 0x76(r30)
    b @L_801C3E24
    @L_801C3E00:
    addi r7, r7, 0x1
    @L_801C3E04:
    clrlwi r0, r7, 16
    cmplwi r0, 0x2
    blt @L_801C3DBC
    addi r6, r6, 0x1
    addi r5, r5, 0x10
    @L_801C3E18:
    clrlwi r0, r6, 16
    cmplwi r0, 0x4
    blt @L_801C3DB4
    @L_801C3E24:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
