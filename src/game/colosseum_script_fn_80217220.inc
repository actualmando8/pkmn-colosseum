    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x10(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r30, r0
    mr r4, r30
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r28, r0
    li r4, 0xe
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217344
    mr r3, r30
    li r4, 0xe
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80217344
    mr r4, r31
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80217344
    mr r7, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0xe
    bl fn_80201C58
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r30
    li r4, 0xe
    bl fn_80202810
    lis r4, lbl_80378EFD@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378EFD@l
    li r5, 0x0
    bl fn_80211B94
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
@L_80217344:
    mr r3, r30
    li r4, 0x1c
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021737C
    mr r3, r30
    li r4, 0x1c
    bl fn_80202810
    lis r4, lbl_80378F11@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378F11@l
    li r5, 0x0
    bl fn_80211B94
@L_8021737C:
    mr r3, r29
    li r4, 0x4a
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802173B4
    mr r3, r29
    li r4, 0x4a
    bl fn_801F6EEC
    lis r4, lbl_80378F25@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378F25@l
    li r5, 0x0
    bl fn_80211B94
@L_802173B4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
