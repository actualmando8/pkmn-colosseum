    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    lis r6, -32729
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    addi r31, r6, 6128
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r5
    stw r28, 0x10(r1)
    mr r28, r4
    bne @L_8010132C
    addi r3, r31, 0x300
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80101380
@L_8010132C:
    addi r4, r31, 0x2c8
    bl fn_80191ECC
    cmplwi r3, 0x0
    bne @L_8010134C
    addi r3, r31, 0x328
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80101380
@L_8010134C:
    lwz r3, 0x4(r3)
    bl fn_800D27FC
    mr. r30, r3
    bne @L_80101368
    addi r3, r31, 0x358
    crxor 6, 6, 6
    bl fn_800DD970
@L_80101368:
    lis r4, -32752
    mr r3, r30
    addi r6, r4, 6696
    mr r5, r29
    mr r4, r28
    bl fn_800F9378
@L_80101380:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
