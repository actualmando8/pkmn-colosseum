    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80057270
    cmplwi r3, 0x0
    bne @L_80053E9C
    li r3, 0x0
    b @L_80053EC8
    @L_80053E9C:
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x0
    li r5, -0x1
    li r6, 0xe7
    bl fn_800FB680
    li r3, 0x0
    @L_80053EC8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
