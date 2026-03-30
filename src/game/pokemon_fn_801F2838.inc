    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r29, r3
    mr r31, r5
    mr r3, r30
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x9
    bne @L_801F2878
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_8011AB50
    @L_801F2878:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
