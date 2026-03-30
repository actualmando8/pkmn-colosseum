    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    beq @L_801F75E4
    clrlwi r31, r4, 16
    li r29, 0x0
    b @L_801F75D8
    @L_801F7554:
    clrlwi r0, r29, 16
    mulli r0, r0, 0x5230
    add. r30, r28, r0
    beq @L_801F75D4
    cmplwi r30, 0x0
    beq @L_801F7578
    mr r3, r30
    li r4, 0x0
    bl fn_801F78AC
    @L_801F7578:
    cmplwi r30, 0x0
    bne @L_801F7588
    li r3, 0x0
    b @L_801F7594
    @L_801F7588:
    mr r3, r30
    li r4, 0x0
    bl fn_801F7908
    @L_801F7594:
    li r4, 0x6
    bl fn_8011B950
    cmplwi r30, 0x0
    bne @L_801F75AC
    li r3, 0x0
    b @L_801F75B8
    @L_801F75AC:
    mr r3, r30
    li r4, 0x0
    bl fn_801F78D4
    @L_801F75B8:
    li r4, 0x2
    bl fn_801FA8CC
    cmplwi r30, 0x0
    beq @L_801F75D4
    mr r3, r30
    li r4, 0x0
    bl fn_801F789C
    @L_801F75D4:
    addi r29, r29, 0x1
    @L_801F75D8:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_801F7554
    @L_801F75E4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
