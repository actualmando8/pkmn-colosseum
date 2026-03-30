    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    bl fn_80128E24
    cmplwi r3, 0x0
    beq @L_8011414C
    bl fn_80128E04
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_8011414C
    mr r5, r28
    li r4, 0x5
    bl fn_80135030
    mr r3, r31
    mr r5, r29
    li r4, 0x7
    bl fn_80135030
    mr r3, r31
    clrlwi r5, r30, 24
    li r4, 0x8
    bl fn_80135030
@L_8011414C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
