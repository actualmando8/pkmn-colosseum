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
    blt @L_800B4F74
    lwz r29, 0xc0(r31)
    lwz r3, 0xc(r29)
    cmpwi r3, 0x0
    bge @L_800B4EA0
    li r28, -0xe
    b @L_800B4F74
    @L_800B4EA0:
    lwz r0, 0xc(r31)
    subf r0, r0, r3
    stw r0, 0xc(r29)
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    bgt @L_800B4F08
    mr r3, r31
    bl fn_800B1E0C
    lwz r5, 0x4(r29)
    lis r4, 0x8000
    lwz r0, 0xf8(r4)
    slwi r4, r5, 6
    add r28, r3, r4
    srwi r29, r0, 2
    bl OSGetTime
    addi r6, r29, 0x0
    li r5, 0x0
    bl fn_800C4928
    stw r4, 0x28(r28)
    li r0, 0x0
    addi r3, r30, 0x0
    lwz r4, 0xd0(r31)
    stw r0, 0xd0(r31)
    bl fn_800B1FAC
    mr r28, r3
    b @L_800B4F6C
    @L_800B4F08:
    mr r3, r31
    bl fn_800B1A08
    lwz r4, 0x8(r29)
    lwz r0, 0xc(r31)
    add r0, r4, r0
    stw r0, 0x8(r29)
    lhz r0, 0x10(r29)
    slwi r0, r0, 1
    lhzx r0, r3, r0
    sth r0, 0x10(r29)
    lhz r4, 0x10(r29)
    cmplwi r4, 0x5
    blt @L_800B4F48
    lhz r0, 0x10(r31)
    cmplw r4, r0
    blt @L_800B4F50
    @L_800B4F48:
    li r28, -0x6
    b @L_800B4F74
    @L_800B4F50:
    lwz r0, 0xc(r31)
    lis r3, fn_800B4FC0@ha
    addi r5, r3, fn_800B4FC0@l
    mullw r4, r0, r4
    addi r3, r30, 0x0
    bl fn_800AFFE0
    mr r28, r3
    @L_800B4F6C:
    cmpwi r28, 0x0
    bge @L_800B4FA0
    @L_800B4F74:
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
    @L_800B4FA0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
