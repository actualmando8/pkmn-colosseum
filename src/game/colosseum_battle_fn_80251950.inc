    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r5
    mr r27, r4
    mr r29, r6
    mr r4, r28
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r31, r3, 16
    mr r4, r28
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r30, r3, 16
    mr r4, r29
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    mr r3, r27
    mr r5, r0
    mr r4, r29
    mr r6, r28
    mr r7, r31
    mr r8, r30
    bl fn_80232110
    mr r0, r3
    mr r3, r27
    mr r31, r0
    li r4, 0x32
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802519F4
    mulli r3, r31, 0xf
    li r0, 0xa
    divw r31, r3, r0
@L_802519F4:
    mr r3, r31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
