    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    addi r29, r3, 0x0
    mulli r6, r29, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r6
    li r0, 0xf1
    stb r0, 0x94(r31)
    extrwi r3, r4, 7, 8
    extrwi r0, r4, 8, 15
    stb r3, 0x95(r31)
    li r6, 0x3
    addi r3, r29, 0x0
    stb r0, 0x96(r31)
    li r0, -0x1
    li r4, 0x0
    stw r6, 0xa0(r31)
    stw r0, 0xa4(r31)
    stw r6, 0xa8(r31)
    bl fn_800AFBDC
    addi r30, r3, 0x0
    cmpwi r30, -0x1
    bne @L_800B0058
    li r30, 0x0
    b @L_800B00A0
    @L_800B0058:
    cmpwi r30, 0x0
    blt @L_800B00A0
    lwz r5, 0xa0(r31)
    addi r3, r29, 0x0
    addi r4, r31, 0x94
    li r6, 0x1
    bl fn_80098368
    cmpwi r3, 0x0
    bne @L_800B008C
    li r0, 0x0
    stw r0, 0xcc(r31)
    li r30, -0x3
    b @L_800B0090
    @L_800B008C:
    li r30, 0x0
    @L_800B0090:
    mr r3, r29
    bl EXIDeselect
    mr r3, r29
    bl EXIUnlock
    @L_800B00A0:
    mr r3, r30
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
