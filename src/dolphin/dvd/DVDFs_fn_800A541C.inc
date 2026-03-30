    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r27, 0x24(r1)
    mr. r29, r6
    addi r31, r3, 0x0
    addi r27, r4, 0x0
    addi r28, r5, 0x0
    addi r30, r7, 0x0
    blt @L_800A5450
    lwz r0, 0x34(r31)
    cmplw r29, r0
    blt @L_800A5468
@L_800A5450:
    lis r3, -32719
    crxor 6, 6, 6
    addi r5, r3, 6692
    addi r3, r13, -32352
    li r4, 0x32c
    bl fn_800060F0
@L_800A5468:
    add. r4, r29, r28
    blt @L_800A5480
    lwz r3, 0x34(r31)
    addi r0, r3, 0x20
    cmplw r4, r0
    blt @L_800A5498
@L_800A5480:
    lis r3, -32719
    crxor 6, 6, 6
    addi r5, r3, 6692
    addi r3, r13, -32352
    li r4, 0x332
    bl fn_800060F0
@L_800A5498:
    lwz r0, 0x30(r31)
    lis r4, -32758
    addi r7, r4, 21812
    addi r3, r31, 0x0
    addi r4, r27, 0x0
    addi r5, r28, 0x0
    addi r8, r30, 0x0
    add r6, r0, r29
    bl fn_800A720C
    cmpwi r3, 0x0
    bne @L_800A54CC
    li r3, -0x1
    b @L_800A5520
@L_800A54CC:
    bl OSDisableInterrupts
    mr r30, r3
@L_800A54D4:
    lwz r0, 0xc(r31)
    cmpwi r0, 0x0
    bne @L_800A54E8
    lwz r31, 0x20(r31)
    b @L_800A5514
@L_800A54E8:
    cmpwi r0, -0x1
    bne @L_800A54F8
    li r31, -0x1
    b @L_800A5514
@L_800A54F8:
    cmpwi r0, 0xa
    bne @L_800A5508
    li r31, -0x3
    b @L_800A5514
@L_800A5508:
    addi r3, r13, -24640
    bl fn_800A238C
    b @L_800A54D4
@L_800A5514:
    mr r3, r30
    bl OSRestoreInterrupts
    mr r3, r31
@L_800A5520:
    lmw r27, 0x24(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
