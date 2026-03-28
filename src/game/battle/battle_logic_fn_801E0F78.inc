    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_801E0FA4
    bl fn_8011E15C
    clrlwi r3, r3, 16
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
@L_801E0FA4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
