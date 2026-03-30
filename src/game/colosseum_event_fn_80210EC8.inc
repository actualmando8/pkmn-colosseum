    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r8, -32712
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    addi r3, r8, -24744
    mr r28, r4
    addis r3, r3, 0x1
    mr r29, r5
    mr r30, r6
    mr r31, r7
    lbz r0, 0x6002(r3)
    cmplwi r0, 0x1
    bne @L_80210FEC
    mr r4, r27
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x4
    beq @L_80210F34
    cmplwi r0, 0x6
    beq @L_80210F34
    cmplwi r0, 0x1
    bne @L_80210F78
@L_80210F34:
    rlwinm. r0, r30, 0, 16, 31
    beq @L_80210F78
    mr r3, r29
    mr r4, r27
    mr r6, r31
    li r5, 0x2
    bl fn_80208750
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r29
    mr r4, r27
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_80211024
@L_80210F78:
    mr r3, r28
    mr r4, r27
    mr r6, r31
    li r5, 0x1
    bl fn_80208750
    mr r3, r29
    mr r4, r27
    mr r6, r31
    li r5, 0x2
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r27
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r29
    mr r4, r27
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_80211024
@L_80210FEC:
    mr r3, r28
    mr r4, r27
    mr r6, r31
    li r5, 0x3
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r27
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80211024:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
