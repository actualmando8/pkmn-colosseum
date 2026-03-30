    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    addi r30, r5, 0x0
    stw r29, 0x1c(r1)
    addi r29, r4, 0x0
    addi r5, r29, 0x0
    stw r28, 0x18(r1)
    addi r28, r3, 0x0
    addi r6, r29, 0x2
    lhz r3, 0x4(r4)
    mulli r4, r28, 0x110
    addi r0, r3, 0x1
    lis r3, lbl_803FC620@ha
    sth r0, 0x4(r29)
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r4
    addi r3, r29, 0x4
    li r4, 0x1ffc
    bl fn_800B2070
    addi r3, r29, 0x0
    li r4, 0x2000
    bl fn_8009B300
    stw r30, 0xd8(r31)
    lis r3, fn_800B1AE4@ha
    addi r5, r3, fn_800B1AE4@l
    lwz r0, 0x80(r31)
    mr r3, r28
    lwz r4, 0xc(r31)
    subf r0, r0, r29
    srwi r0, r0, 13
    mullw r4, r4, r0
    bl fn_800AFFE0
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    lwz r28, 0x18(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
