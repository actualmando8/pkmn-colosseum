    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r5
    mr r31, r3
    lwz r27, 0x0(r5)
    mr r3, r27
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x10
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r28, r3, 0, 24, 31
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A55C
    li r3, 0x1
    b @L_8022A6B4
@L_8022A55C:
    mr r3, r31
    li r4, 0x33
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A6B0
    cmplwi r28, 0x1
    bne @L_8022A6B0
    cmplwi r27, 0x0
    beq @L_8022A674
    cmplwi r31, 0x0
    beq @L_8022A674
    mr r3, r27
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2e
    bne @L_8022A674
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x121
    mr r28, r3
    bl fn_80123B5C
    mr r29, r3
    extsb. r0, r29
    blt @L_8022A674
    extsb r27, r29
    mr r3, r28
    mr r6, r27
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm. r4, r3, 0, 24, 31
    beq @L_8022A5E8
    subi r0, r4, 0x1
    rlwinm r4, r0, 0, 24, 31
@L_8022A5E8:
    mr r3, r28
    mr r6, r27
    rlwinm r7, r4, 0, 24, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_801254B4
    mr r3, r31
    rlwinm r28, r29, 0, 24, 31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A674
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A674
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    li r0, 0x1
    slw r0, r0, r28
    and. r0, r3, r0
    bne @L_8022A674
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A674
    mr r3, r31
    mr r5, r28
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_8022A674:
    mr r3, r31
    li r4, 0x33
    bl fn_80202810
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_8037919D@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037919D@l
    li r5, 0x0
    bl fn_80211B94
    stw r31, 0x4(r30)
@L_8022A6B0:
    li r3, 0x1
@L_8022A6B4:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
