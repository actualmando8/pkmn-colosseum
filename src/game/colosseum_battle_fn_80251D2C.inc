    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    mr r3, r31
    li r5, 0xd9
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80205B8C
    addi r4, r1, 0xa
    addi r5, r1, 0x8
    bl fn_80120B00
    lhz r7, 0xa(r1)
    mr r3, r31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lhz r7, 0x8(r1)
    mr r3, r31
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BBD8
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
