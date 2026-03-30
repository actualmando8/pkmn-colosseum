    nofralloc
    mflr r0
    li r6, 0x5
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    mulli r5, r30, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    li r0, 0x52
    stb r0, 0x94(r31)
    li r0, 0x0
    addi r3, r30, 0x0
    lwz r5, 0xb0(r31)
    extrwi r5, r5, 7, 8
    stb r5, 0x95(r31)
    li r5, 0x0
    lwz r7, 0xb0(r31)
    extrwi r7, r7, 8, 15
    stb r7, 0x96(r31)
    lwz r7, 0xb0(r31)
    extrwi r7, r7, 2, 23
    stb r7, 0x97(r31)
    lwz r7, 0xb0(r31)
    clrlwi r7, r7, 25
    stb r7, 0x98(r31)
    stw r6, 0xa0(r31)
    stw r0, 0xa4(r31)
    stw r0, 0xa8(r31)
    bl fn_800AFBDC
    cmpwi r3, -0x1
    bne @L_800AFE20
    li r3, 0x0
    b @L_800AFEAC
    @L_800AFE20:
    cmpwi r3, 0x0
    blt @L_800AFEAC
    lwz r5, 0xa0(r31)
    addi r3, r30, 0x0
    addi r4, r31, 0x94
    li r6, 0x1
    bl fn_80098368
    cmpwi r3, 0x0
    beq @L_800AFE88
    lwz r4, 0x80(r31)
    mr r3, r30
    lwz r5, 0x14(r31)
    li r6, 0x1
    addi r4, r4, 0x200
    bl fn_80098368
    cmpwi r3, 0x0
    beq @L_800AFE88
    lis r3, fn_800AF474@ha
    lwz r4, 0xb4(r31)
    addi r7, r3, fn_800AF474@l
    lwz r6, 0xa4(r31)
    addi r3, r30, 0x0
    li r5, 0x200
    bl EXIDma
    cmpwi r3, 0x0
    bne @L_800AFEA8
    @L_800AFE88:
    li r0, 0x0
    stw r0, 0xc8(r31)
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFEAC
    @L_800AFEA8:
    li r3, 0x0
    @L_800AFEAC:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
