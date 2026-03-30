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
    mr r5, r29
    rlwinm r4, r31, 0, 16, 31
    bl fn_801DA9E8
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80209548
@L_80209518:
    mr r3, r28
    mr r5, r29
    rlwinm r4, r31, 0, 16, 31
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80209538
    bl fn_800F0308
    b @L_80209518
@L_80209538:
    mr r3, r28
    mr r5, r29
    rlwinm r4, r31, 0, 16, 31
    bl fn_801DA8C4
@L_80209548:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
