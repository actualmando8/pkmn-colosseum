    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r30, r5
    mr r29, r6
    li r3, 0x0
    li r4, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r3, r3, 16
    bl fn_801363E8
    mr r4, r30
    mr r30, r3
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r31, r3, 16
    mr r4, r30
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r0, r3, 16
    cmplw r0, r31
    beq @L_802552B0
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    bl fn_8023C530
    b @L_802552B4
@L_802552B0:
    li r3, 0x1
@L_802552B4:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
