    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80057270
    cmplwi r3, 0x0
    beq @L_80053DB4
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E760
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x0
    li r5, -0x1
    li r6, 0xe7
    bl fn_800FB680
    @L_80053DB4:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x4c(r31)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
