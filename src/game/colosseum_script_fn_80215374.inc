    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    bl fn_801F025C
    li r4, 0x27
    mr r31, r3
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_802153B8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80215514
@L_802153B8:
    cmplwi r31, 0x0
    beq @L_802154B8
    mr r6, r31
    li r3, 0x0
    li r4, 0x2e
    li r5, 0x2
    bl fn_801F3624
    rlwinm. r29, r3, 0, 24, 31
    beq @L_802154B8
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x11e
    mr r28, r3
    bl fn_80123B5C
    mr r30, r3
    extsb. r0, r30
    blt @L_802154B8
    extsb r27, r30
    mr r3, r28
    mr r6, r27
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    subfc r5, r0, r29
    mr r6, r27
    subf r0, r29, r0
    li r4, 0x0
    subfe r7, r5, r5
    li r5, 0x80
    rlwinm r0, r0, 0, 24, 31
    and r0, r0, r7
    rlwinm r7, r0, 0, 24, 31
    bl fn_801254B4
    mr r3, r31
    rlwinm r28, r30, 0, 24, 31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802154B8
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802154B8
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    li r0, 0x1
    slw r0, r0, r28
    and. r0, r3, r0
    bne @L_802154B8
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802154B8
    mr r3, r31
    mr r5, r28
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_802154B8:
    li r0, 0x0
    lis r3, -32735
    stw r31, 0x8(r1)
    addi r4, r3, 21800
    addi r5, r1, 0x8
    li r3, 0x0
    stw r0, 0xc(r1)
    li r6, 0x0
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    bne @L_802154F8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80215514
@L_802154F8:
    mr r3, r31
    li r4, 0x27
    li r5, 0x0
    bl fn_8020248C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80215514:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
