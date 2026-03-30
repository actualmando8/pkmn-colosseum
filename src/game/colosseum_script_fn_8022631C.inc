    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_802037DC
    mr r3, r30
    bl fn_802040E8
    li r0, 0x0
    la r31, lbl_80478D78(r13)
    stb r0, 0x7(r31)
    mr r27, r3
    mr r3, r29
    li r28, 0x0
    li r4, 0x40
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802263AC
    mr r3, r29
    li r4, 0x43
    bl fn_8020990C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802263D4
@L_802263AC:
    mr r3, r29
    li r4, 0x40
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802263F0
    la r3, lbl_80478D78(r13)
    lbz r0, 0x6(r3)
    cmplwi r0, 0x3
    blt @L_802263F0
@L_802263D4:
    la r4, lbl_80478D78(r13)
    lis r3, lbl_80279D18@ha
    lbz r0, 0x6(r4)
    addi r3, r3, lbl_80279D18@l
    rlwinm r0, r0, 2, 0, 29
    lwzx r28, r3, r0
    b @L_80226688
@L_802263F0:
    mr r3, r29
    bl fn_80209618
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80226538
    mr r3, r29
    li r4, 0x41
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226420
    li r28, 0x7637
    b @L_80226688
@L_80226420:
    mr r3, r29
    li r4, 0x42
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226440
    li r28, 0x7636
    b @L_80226688
@L_80226440:
    mr r3, r29
    li r4, 0x43
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226460
    li r28, 0x7543
    b @L_80226688
@L_80226460:
    mr r3, r29
    li r4, 0x44
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226480
    li r28, 0x7632
    b @L_80226688
@L_80226480:
    mr r3, r29
    li r4, 0x45
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802264A0
    li r28, 0x7647
    b @L_80226688
@L_802264A0:
    mr r3, r29
    li r4, 0x46
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802264C0
    li r28, 0x75c9
    b @L_80226688
@L_802264C0:
    mr r3, r29
    li r4, 0x47
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226688
    mr r3, r29
    li r4, 0x46
    bl fn_80209960
    mr r3, r29
    li r4, 0x47
    bl fn_80209960
    rlwinm r7, r27, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80379BE7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379BE7@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802266D8
@L_80226538:
    mr r3, r29
    li r4, 0x43
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226558
    li r28, 0x7543
    b @L_80226688
@L_80226558:
    mr r3, r29
    li r4, 0x44
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802265AC
    mr r3, r29
    li r4, 0x44
    bl fn_80209960
    mr r3, r29
    li r4, 0x41
    bl fn_80209960
    mr r3, r29
    li r4, 0x42
    bl fn_80209960
    lis r4, lbl_803791C7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803791C7@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802266D8
@L_802265AC:
    mr r3, r29
    li r4, 0x46
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802265F4
    mr r3, r29
    li r4, 0x46
    bl fn_80209960
    mr r3, r29
    li r4, 0x47
    bl fn_80209960
    lis r4, lbl_803791BE@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803791BE@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802266D8
@L_802265F4:
    mr r3, r29
    li r4, 0x47
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022666C
    mr r3, r29
    li r4, 0x46
    bl fn_80209960
    mr r3, r29
    li r4, 0x47
    bl fn_80209960
    rlwinm r7, r27, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80379BE7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379BE7@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802266D8
@L_8022666C:
    mr r3, r29
    li r4, 0x45
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226688
    li r28, 0x7647
@L_80226688:
    cmplwi r28, 0x0
    beq @L_802266CC
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x53
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r28
    mr r4, r30
    mr r5, r27
    bl fn_80262334
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802266CC
    li r0, 0x1
    stb r0, 0x7(r31)
@L_802266CC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_802266D8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
