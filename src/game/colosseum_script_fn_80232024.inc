    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023204C
    li r3, 0x1
    b @L_802320FC
@L_8023204C:
    mr r3, r30
    li r4, 0x35
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802320F8
    mr r3, r30
    li r4, 0x35
    bl fn_80202234
    mr r31, r3
    mr r3, r30
    li r4, 0x35
    bl fn_80202108
    extsb r3, r3
    extsb r0, r31
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_802320A8
    mr r3, r30
    li r4, 0x35
    bl fn_80201FDC
    b @L_802320F8
@L_802320A8:
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80379052@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379052@l
    li r5, 0x0
    bl fn_80211B94
    mr r3, r30
    li r4, 0x35
    bl fn_80202810
@L_802320F8:
    li r3, 0x1
@L_802320FC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
