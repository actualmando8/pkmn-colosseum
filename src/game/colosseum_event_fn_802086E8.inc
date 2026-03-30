    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r5
    li r3, 0x0
    li r5, 0x1f
    bl fn_8011BEB4
    mr r31, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_8020873C
    mr r5, r30
    rlwinm r4, r31, 0, 16, 31
    bl fn_801DA8C4
@L_8020873C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
