    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    lis r7, -32729
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r4
    mr r30, r5
    mr r28, r6
    addi r31, r7, 6128
    bne @L_801013DC
    addi r3, r31, 0x3f4
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80101468
@L_801013DC:
    addi r4, r31, 0x2c8
    bl fn_80191ECC
    cmplwi r3, 0x0
    bne @L_801013FC
    addi r3, r31, 0x418
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80101468
@L_801013FC:
    lwz r4, 0x0(r3)
    li r29, 0x0
    mr r3, r4
    b @L_8010145C
@L_8010140C:
    cmplw r29, r30
    bne @L_80101454
    rlwinm r0, r29, 2, 0, 29
    lwzx r3, r4, r0
    bl fn_800E4D18
    mr. r30, r3
    bne @L_80101438
    mr r4, r29
    addi r3, r31, 0x448
    crxor 6, 6, 6
    bl fn_800DD970
@L_80101438:
    lis r4, -32752
    mr r3, r30
    addi r6, r4, 6732
    mr r5, r28
    mr r4, r27
    bl fn_800F9378
    b @L_80101468
@L_80101454:
    addi r3, r3, 0x4
    addi r29, r29, 0x1
@L_8010145C:
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8010140C
@L_80101468:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
