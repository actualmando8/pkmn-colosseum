    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r6, 0x0
    stw r30, 0x20(r1)
    addi r30, r5, 0x0
    stw r29, 0x1c(r1)
    mr. r29, r4
    stw r28, 0x18(r1)
    addi r28, r3, 0x0
    blt @L_800A5594
    lwz r0, 0x34(r28)
    cmplw r29, r0
    blt @L_800A55AC
@L_800A5594:
    lis r3, -32719
    crxor 6, 6, 6
    addi r5, r3, 6740
    addi r3, r13, -32352
    li r4, 0x383
    bl fn_800060F0
@L_800A55AC:
    stw r30, 0x38(r28)
    lis r3, -32758
    addi r5, r3, 22000
    lwz r0, 0x30(r28)
    addi r3, r28, 0x0
    addi r6, r31, 0x0
    add r4, r0, r29
    bl fn_800A72E8
    lwz r0, 0x2c(r1)
    li r3, 0x1
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    lwz r28, 0x18(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
