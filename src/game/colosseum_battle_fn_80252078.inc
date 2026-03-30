    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r5
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x46
    bl fn_8011BBD8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
