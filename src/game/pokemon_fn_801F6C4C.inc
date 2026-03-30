    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    mr r3, r31
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x6
    beq @L_801F6C80
    li r3, 0x0
    b @L_801F6C8C
    @L_801F6C80:
    mr r3, r30
    mr r4, r31
    bl fn_8011A860
    @L_801F6C8C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
