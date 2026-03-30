    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r4, r27
    li r3, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_8011BEB4
    mr. r31, r3
    bne @L_802095A4
    lis r3, -32735
    addi r0, r3, 4452
    mr r31, r0
@L_802095A4:
    mr r4, r27
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_802095C8
    lis r3, -32735
    addi r3, r3, 4456
@L_802095C8:
    mr r12, r3
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mtctr r12
    bctrl
    mr r12, r31
    mr r7, r3
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mtctr r12
    bctrl
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
