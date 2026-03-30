    nofralloc
    mflr r0
    li r7, 0x5
    stw r0, 0x4(r1)
    li r6, 0x1
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    mulli r5, r30, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    li r0, 0xf2
    stb r0, 0x94(r31)
    addi r5, r4, 0x0
    li r0, 0x3
    lwz r3, 0xb0(r31)
    li r4, 0x0
    extrwi r3, r3, 7, 8
    stb r3, 0x95(r31)
    addi r3, r30, 0x0
    lwz r8, 0xb0(r31)
    extrwi r8, r8, 8, 15
    stb r8, 0x96(r31)
    lwz r8, 0xb0(r31)
    extrwi r8, r8, 2, 23
    stb r8, 0x97(r31)
    lwz r8, 0xb0(r31)
    clrlwi r8, r8, 25
    stb r8, 0x98(r31)
    stw r7, 0xa0(r31)
    stw r6, 0xa4(r31)
    stw r0, 0xa8(r31)
    bl fn_800AFBDC
    cmpwi r3, -0x1
    bne @L_800AFF5C
    li r3, 0x0
    b @L_800AFFC8
    @L_800AFF5C:
    cmpwi r3, 0x0
    blt @L_800AFFC8
    lwz r5, 0xa0(r31)
    addi r3, r30, 0x0
    addi r4, r31, 0x94
    li r6, 0x1
    bl fn_80098368
    cmpwi r3, 0x0
    beq @L_800AFFA4
    lis r3, fn_800AF474@ha
    lwz r4, 0xb4(r31)
    addi r7, r3, fn_800AF474@l
    lwz r6, 0xa4(r31)
    addi r3, r30, 0x0
    li r5, 0x80
    bl EXIDma
    cmpwi r3, 0x0
    bne @L_800AFFC4
    @L_800AFFA4:
    li r0, 0x0
    stw r0, 0xcc(r31)
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFFC8
    @L_800AFFC4:
    li r3, 0x0
    @L_800AFFC8:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
