    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    bne @L_801B0444
    lis r3, 0x8027
    li r4, 0x366
    addi r3, r3, 21184
    addi r5, r2, -22760
    bl fn_80196E10
@L_801B0444:
    lwz r6, 0x0(r28)
    mr r3, r29
    lwz r0, 0x4(r28)
    mr r4, r28
    addi r5, r27, 0x24
    stw r6, 0x0(r27)
    stw r0, 0x4(r27)
    lwz r0, 0x8(r28)
    stw r0, 0x8(r27)
    bl fn_800A3A9C
    addi r3, r27, 0x24
    addi r4, r27, 0x30
    bl fn_800A3ADC
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_800A3ADC
    addi r3, r27, 0x30
    addi r4, r1, 0x8
    addi r5, r27, 0x18
    bl fn_800A3B9C
    addi r3, r27, 0x18
    addi r4, r27, 0x30
    addi r5, r27, 0xc
    bl fn_800A3B9C
    addi r3, r27, 0x24
    bl fn_800A3B38
    lfs f2, -22748(r2)
    stfs f1, 0x3c(r27)
    lfs f0, -22744(r2)
    stfs f2, 0x4c(r27)
    stfs f2, 0x40(r27)
    stfs f0, 0x48(r27)
    stfs f0, 0x44(r27)
    stw r31, 0x50(r27)
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
