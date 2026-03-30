    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x34
    stw r0, 0x24(r1)
    li r6, 0x0
    stmw r29, 0x14(r1)
    mr r29, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 24, 31
    bl fn_80077AF4
    mr r31, r3
    mr r4, r29
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r30, 0x1
    rlwinm r3, r3, 0, 16, 31
    bne @L_802296EC
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802296EC
    cmplwi r3, 0x7
    bne @L_802296EC
    li r3, 0x1
    b @L_802296F0
@L_802296EC:
    li r3, 0x0
@L_802296F0:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
