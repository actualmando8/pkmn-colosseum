    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r27, 0xc(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r4, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_801363E8
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    li r5, 0x9
    mr r4, r28
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80209C1C
    mr r4, r28
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    mr r3, r30
    mr r4, r28
    mr r5, r31
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_80377EA4@ha
    addi r4, r3, lbl_80377EA4@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 0, 22, 20
    li r5, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
    lis r3, lbl_80379BFF@ha
    clrlslwi r0, r27, 16, 2
    addi r3, r3, lbl_80379BFF@l
    lwzx r0, r3, r0
    stw r0, lbl_8047B610(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
