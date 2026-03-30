    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r5
    mr r28, r3
    li r5, 0xd9
    mr r3, r29
    bl fn_8012640C
    li r4, 0x0
    mr r31, r3
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r30, r3, 16
    mr r3, r28
    mr r4, r29
    li r5, 0x1a
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025243C
    clrlslwi r30, r30, 17, 1
@L_8025243C:
    mr r3, r31
    clrlwi r7, r30, 16
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
