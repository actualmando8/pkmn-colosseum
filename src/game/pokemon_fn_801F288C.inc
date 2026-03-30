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
    cmplwi r0, 0x9
    beq @L_801F28C0
    li r3, -0x1
    b @L_801F28CC
    @L_801F28C0:
    mr r3, r30
    mr r4, r31
    bl fn_8011ACB4
    @L_801F28CC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
