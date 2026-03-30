    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r3, r30
    bl fn_80207BF4
    mr r31, r3
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022D380
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x1c
    bne @L_8022D380
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 17, 17
    beq @L_8022D380
    lbz r0, lbl_8047B628(r13)
    rlwinm r3, r3, 0, 18, 16
    stw r3, lbl_8047B618(r13)
    rlwinm r0, r0, 0, 26, 31
    cmplwi r0, 0x6
    stb r0, lbl_8047B628(r13)
    bne @L_8022D338
    li r0, 0x2
    stb r0, lbl_8047B628(r13)
@L_8022D338:
    lbz r4, lbl_8047B628(r13)
    la r5, lbl_80478D78(r13)
    mr r7, r30
    li r3, 0x0
    addi r0, r4, 0x40
    li r4, 0x0
    stb r0, 0x3(r5)
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_80379945@ha
    addi r4, r3, lbl_80379945@l
    lwz r3, lbl_8047B62C(r13)
    ori r0, r0, 0x2000
    li r5, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
@L_8022D380:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
