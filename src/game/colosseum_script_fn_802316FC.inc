    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_80207BF4
    mr r31, r3
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231730
    li r3, 0x1
    b @L_802317D0
@L_80231730:
    mr r3, r30
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802317CC
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2b
    beq @L_802317CC
    mr r3, r30
    li r4, 0x8
    bl fn_80202810
    mr r3, r30
    li r4, 0x17
    bl fn_80202810
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80379249@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379249@l
    li r5, 0x0
    bl fn_80211B94
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802317CC
    mr r3, r30
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_802317CC:
    li r3, 0x1
@L_802317D0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
