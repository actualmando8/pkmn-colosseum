    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r3, 0x0
    lis r3, lbl_803FC620@ha
    stw r29, 0x14(r1)
    mulli r5, r30, 0x110
    stw r28, 0x10(r1)
    addi r0, r3, lbl_803FC620@l
    mr. r28, r4
    add r31, r0, r5
    blt @L_800B3B1C
    lwz r3, 0x28(r31)
    addi r0, r3, 0x1
    stw r0, 0x28(r31)
    lwz r4, 0x28(r31)
    cmpwi r4, 0x5
    bge @L_800B3A98
    lwz r0, 0xc(r31)
    lis r3, fn_800B3A24@ha
    addi r5, r3, fn_800B3A24@l
    mullw r4, r0, r4
    addi r3, r30, 0x0
    bl fn_800AFFE0
    mr. r28, r3
    blt @L_800B3B1C
    b @L_800B3B48
    @L_800B3A98:
    cmpwi r4, 0xa
    bge @L_800B3AD8
    lwz r0, 0xc(r31)
    subi r6, r4, 0x5
    lis r3, fn_800B3A24@ha
    lwz r5, 0x80(r31)
    mullw r4, r0, r6
    slwi r0, r6, 13
    addi r7, r3, fn_800B3A24@l
    add r6, r5, r0
    addi r3, r30, 0x0
    li r5, 0x2000
    bl fn_800B19A4
    mr. r28, r3
    blt @L_800B3B1C
    b @L_800B3B48
    @L_800B3AD8:
    lwz r3, 0x80(r31)
    li r5, 0x2000
    addi r0, r3, 0x2000
    stw r0, 0x84(r31)
    lwz r4, 0x80(r31)
    lwz r3, 0x84(r31)
    addi r4, r4, 0x4000
    bl memcpy
    lwz r3, 0x80(r31)
    li r5, 0x2000
    addi r0, r3, 0x6000
    stw r0, 0x88(r31)
    lwz r4, 0x80(r31)
    lwz r3, 0x88(r31)
    addis r4, r4, 0x1
    addi r4, r4, -0x8000
    bl memcpy
    @L_800B3B1C:
    lwz r29, 0xd0(r31)
    li r0, 0x0
    addi r3, r31, 0x0
    stw r0, 0xd0(r31)
    mr r4, r28
    bl fn_800B02F4
    addi r12, r29, 0x0
    mtlr r12
    addi r3, r30, 0x0
    addi r4, r28, 0x0
    blrl
    @L_800B3B48:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
