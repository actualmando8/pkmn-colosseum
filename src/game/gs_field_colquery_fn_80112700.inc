    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80115BD8
    bl fn_801159A8
    cmplwi r3, 0x0
    beq @L_8011273C
    mr r4, r3
    li r3, 0xf
    li r5, 0x1000
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_800F7318
@L_8011273C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
