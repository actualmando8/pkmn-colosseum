    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r5
    mr r28, r4
    mr r31, r3
    lwz r30, 0x0(r5)
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_8025C6F0
    li r3, 0x1
    b @L_8025C75C
@L_8025C6F0:
    mr r3, r30
    mr r4, r31
    mr r5, r28
    bl fn_801F8424
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C714
    li r3, 0x1
    b @L_8025C75C
@L_8025C714:
    mr r3, r30
    mr r4, r31
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025C758
    mr r3, r30
    mr r4, r31
    li r5, 0x2b
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025C758
    lwz r3, 0x4(r29)
    addi r0, r3, 0x1
    stw r0, 0x4(r29)
@L_8025C758:
    li r3, 0x1
@L_8025C75C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
