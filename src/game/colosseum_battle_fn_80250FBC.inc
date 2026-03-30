    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r5
    mr r28, r3
    li r5, 0xd9
    mr r3, r31
    bl fn_8012640C
    mr r0, r3
    mr r3, r28
    mr r29, r0
    mr r4, r31
    bl fn_802376EC
    mr r30, r3
    mr r3, r28
    mr r4, r31
    bl fn_80237664
    mr r31, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r3, r3, 16
    clrlwi r0, r30, 16
    mullw r3, r3, r0
    clrlwi r0, r31, 16
    divw r0, r3, r0
    clrlwi. r0, r0, 16
    bne @L_80251044
    li r0, 0x1
@L_80251044:
    mr r3, r29
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
