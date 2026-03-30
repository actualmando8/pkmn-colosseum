    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r3
    bl fn_80207BF4
    mr r31, r3
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022E22C
    li r3, 0x1
    b @L_8022E300
@L_8022E22C:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x24
    bne @L_8022E2FC
    mr r3, r30
    li r4, 0x3c
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E2FC
    mr r6, r30
    li r3, 0x0
    li r4, 0x1
    li r5, 0x2
    bl fn_801F2598
    mr r31, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E2FC
    mr r3, r30
    li r4, 0x3c
    bl fn_80202810
    mr r3, r31
    bl fn_80207BF4
    mr r0, r3
    mr r3, r30
    mr r29, r0
    mr r4, r29
    bl fn_80207BC0
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r29
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    lis r4, lbl_80379667@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379667@l
    li r5, 0x0
    bl fn_80211B94
@L_8022E2FC:
    li r3, 0x1
@L_8022E300:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
