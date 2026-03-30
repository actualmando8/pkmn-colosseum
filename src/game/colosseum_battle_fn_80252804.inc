    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r5
    mr r30, r3
    li r5, 0xd9
    mr r3, r29
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r31, r0
    mr r4, r29
    bl fn_802376EC
    mr r0, r3
    mr r3, r30
    mr r30, r0
    mr r4, r29
    bl fn_80237664
    mr r0, r3
    mr r3, r30
    mr r4, r0
    bl fn_80218B6C
    clrlwi r7, r3, 24
    mr r3, r31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
