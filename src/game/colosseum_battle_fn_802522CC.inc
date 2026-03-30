    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r5
    mr r29, r3
    li r5, 0xd9
    mr r3, r30
    bl fn_8012640C
    li r4, 0x0
    mr r31, r3
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    mr r3, r29
    mr r4, r30
    bl fn_8023842C
    bl fn_80217BEC
    mr r0, r3
    mr r3, r31
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
