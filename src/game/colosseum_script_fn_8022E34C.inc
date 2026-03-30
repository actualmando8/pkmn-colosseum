    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r5
    mr r29, r3
    bl fn_80207BF4
    mr r30, r3
    mr r3, r29
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022E384
    li r3, 0x1
    b @L_8022E3FC
@L_8022E384:
    lbz r0, 0x0(r31)
    lis r3, lbl_803795FE@ha
    addi r31, r3, lbl_803795FE@l
    cmplwi r0, 0x1
    bne @L_8022E3A0
    lis r3, lbl_803795F5@ha
    addi r31, r3, lbl_803795F5@l
@L_8022E3A0:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x16
    bne @L_8022E3F8
    mr r3, r29
    li r4, 0x3b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E3F8
    mr r3, r29
    li r4, 0x3b
    bl fn_80202810
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B62C(r13)
    mr r4, r31
    li r5, 0x0
    bl fn_80211B94
@L_8022E3F8:
    li r3, 0x1
@L_8022E3FC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
