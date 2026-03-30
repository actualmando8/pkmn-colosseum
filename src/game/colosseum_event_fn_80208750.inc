    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    mr r29, r5
    mr r30, r6
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_8011BEB4
    mr r31, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_802087AC
    mr r5, r29
    mr r6, r30
    rlwinm r4, r31, 0, 16, 31
    bl fn_801DDD28
@L_802087AC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
