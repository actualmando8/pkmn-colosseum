    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r5
    mr r31, r3
    mr r3, r30
    bl fn_80207BF4
    mr r0, r3
    mr r3, r31
    mr r29, r0
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80214D3C
    li r3, 0x1
    b @L_80214D9C
@L_80214D3C:
    mr r3, r31
    mr r4, r30
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80214D98
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r4, lbl_8037960A@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037960A@l
    li r5, 0x0
    bl fn_80211B94
@L_80214D98:
    li r3, 0x1
@L_80214D9C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
