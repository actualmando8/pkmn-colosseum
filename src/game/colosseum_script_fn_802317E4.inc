    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80231FB4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r30, r3, 0, 24, 31
    li r3, 0x0
    bl fn_801F4718
    li r3, 0x0
    li r4, 0x0
    bl fn_801F3B24
    li r0, 0x0
    lis r3, -32733
    stw r0, 0x8(r1)
    addi r4, r3, 8136
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r31, 0x0
    b @L_80231948
@L_8023184C:
    rlwinm r4, r31, 0, 24, 31
    li r3, 0x0
    bl fn_801F47B4
    mr. r28, r3
    beq @L_80231944
    mr r4, r28
    li r3, 0xc
    bl fn_801F025C
    mr r27, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023188C
    mr r4, r28
    li r3, 0xd
    bl fn_801F025C
    mr r27, r3
@L_8023188C:
    mr r7, r27
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r28
    li r4, 0x48
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231944
    mr r3, r28
    li r4, 0x48
    bl fn_801F6D48
    mr r29, r3
    mr r3, r28
    li r4, 0x48
    bl fn_801F6CF4
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231900
    mr r3, r28
    li r4, 0x48
    bl fn_801F6CA0
    b @L_80231944
@L_80231900:
    mr r3, r28
    li r4, 0x48
    bl fn_801F6EEC
    li r3, 0x0
    li r4, 0x73
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r4, lbl_80378B30@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378B30@l
    li r5, 0x0
    bl fn_80211B94
@L_80231944:
    addi r31, r31, 0x1
@L_80231948:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8023184C
    bl fn_801DA7AC
    li r28, 0x0
    la r31, lbl_80478D78(r13)
    b @L_80231A64
@L_80231964:
    rlwinm r4, r28, 0, 24, 31
    li r3, 0x0
    bl fn_801F47B4
    mr. r27, r3
    beq @L_80231A60
    mr r4, r27
    li r3, 0xc
    bl fn_801F025C
    mr r29, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802319A4
    mr r4, r27
    li r3, 0xd
    bl fn_801F025C
    mr r29, r3
@L_802319A4:
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r27
    li r4, 0x49
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231A60
    mr r3, r27
    li r4, 0x49
    bl fn_801F6D48
    mr r29, r3
    mr r3, r27
    li r4, 0x49
    bl fn_801F6CF4
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231A18
    mr r3, r27
    li r4, 0x49
    bl fn_801F6CA0
    b @L_80231A60
@L_80231A18:
    mr r3, r27
    li r4, 0x49
    bl fn_801F6EEC
    li r3, 0x0
    li r4, 0x71
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r3, lbl_80378B30@ha
    stb r28, 0x5(r31)
    addi r4, r3, lbl_80378B30@l
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
@L_80231A60:
    addi r28, r28, 0x1
@L_80231A64:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80231964
    bl fn_801DA7AC
    li r28, 0x0
    b @L_80231B78
@L_80231A7C:
    rlwinm r4, r28, 0, 24, 31
    li r3, 0x0
    bl fn_801F47B4
    mr. r27, r3
    beq @L_80231B74
    mr r4, r27
    li r3, 0xc
    bl fn_801F025C
    mr r29, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231ABC
    mr r4, r27
    li r3, 0xd
    bl fn_801F025C
    mr r29, r3
@L_80231ABC:
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r27
    li r4, 0x4c
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231B74
    mr r3, r27
    li r4, 0x4c
    bl fn_801F6D48
    mr r29, r3
    mr r3, r27
    li r4, 0x4c
    bl fn_801F6CF4
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231B30
    mr r3, r27
    li r4, 0x4c
    bl fn_801F6CA0
    b @L_80231B74
@L_80231B30:
    mr r3, r27
    li r4, 0x4c
    bl fn_801F6EEC
    li r3, 0x0
    li r4, 0x36
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r4, lbl_80378B30@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378B30@l
    li r5, 0x0
    bl fn_80211B94
@L_80231B74:
    addi r28, r28, 0x1
@L_80231B78:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80231A7C
    bl fn_801DA7AC
    li r28, 0x0
    b @L_80231C68
@L_80231B90:
    rlwinm r4, r28, 0, 24, 31
    li r3, 0x0
    bl fn_801F47B4
    mr. r27, r3
    beq @L_80231C64
    mr r4, r27
    li r3, 0xc
    bl fn_801F025C
    mr r29, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231BD0
    mr r4, r27
    li r3, 0xd
    bl fn_801F025C
    mr r29, r3
@L_80231BD0:
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r27
    li r4, 0x4b
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231C64
    mr r3, r27
    li r4, 0x4b
    bl fn_801F6D48
    mr r29, r3
    mr r3, r27
    li r4, 0x4b
    bl fn_801F6CF4
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231C44
    mr r3, r27
    li r4, 0x4b
    bl fn_801F6CA0
    b @L_80231C64
@L_80231C44:
    mr r3, r27
    li r4, 0x4b
    bl fn_801F6EEC
    lis r4, lbl_80378B5B@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378B5B@l
    li r5, 0x0
    bl fn_80211B94
@L_80231C64:
    addi r28, r28, 0x1
@L_80231C68:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80231B90
    bl fn_801DA7AC
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 8228
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    bl fn_801DA7AC
    cmplwi r30, 0x2
    bne @L_80231D48
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x50
    bl fn_801F29DC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231D2C
    li r3, 0x0
    li r4, 0x54
    bl fn_801F28E0
    mr r29, r3
    li r3, 0x0
    li r4, 0x54
    bl fn_801F288C
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231D14
    li r3, 0x0
    li r4, 0x54
    bl fn_801F2838
    li r0, 0x0
    stb r0, 0x5(r31)
    b @L_80231D34
@L_80231D14:
    li r3, 0x0
    li r4, 0x54
    bl fn_801F2A30
    li r0, 0x2
    stb r0, 0x5(r31)
    b @L_80231D34
@L_80231D2C:
    li r0, 0x0
    stb r0, 0x5(r31)
@L_80231D34:
    lis r4, lbl_80378A5F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378A5F@l
    li r5, 0x0
    bl fn_80211B94
@L_80231D48:
    bl fn_801DA7AC
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80231FB4
    cmplwi r30, 0x3
    bne @L_80231E30
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x51
    bl fn_801F29DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231DA0
    lis r3, lbl_80378968@ha
    addi r0, r3, lbl_80378968@l
    mr r4, r0
    b @L_80231E08
@L_80231DA0:
    li r3, 0x0
    li r4, 0x55
    bl fn_801F28E0
    mr r29, r3
    li r3, 0x0
    li r4, 0x55
    bl fn_801F288C
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231DF0
    li r3, 0x0
    li r4, 0x55
    bl fn_801F2838
    lis r3, lbl_80378968@ha
    addi r0, r3, lbl_80378968@l
    mr r4, r0
    b @L_80231E08
@L_80231DF0:
    li r3, 0x0
    li r4, 0x55
    bl fn_801F2A30
    lis r3, lbl_80378A4D@ha
    addi r0, r3, lbl_80378A4D@l
    mr r4, r0
@L_80231E08:
    lis r3, lbl_80379F58@ha
    li r0, 0x0
    addi r3, r3, lbl_80379F58@l
    li r6, 0xc
    addis r5, r3, 0x1
    stb r0, 0x5(r31)
    lwz r3, lbl_8047B62C(r13)
    stb r6, 0x60a4(r5)
    li r5, 0x0
    bl fn_80211B94
@L_80231E30:
    bl fn_801DA7AC
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80231FB4
    cmplwi r30, 0x1
    bne @L_80231EF0
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x4f
    bl fn_801F29DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231E84
    lis r3, lbl_80378A7C@ha
    addi r4, r3, lbl_80378A7C@l
    b @L_80231EE4
@L_80231E84:
    li r3, 0x0
    li r4, 0x53
    bl fn_801F28E0
    mr r29, r3
    li r3, 0x0
    li r4, 0x53
    bl fn_801F288C
    extsb r3, r3
    extsb r0, r29
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231ED0
    li r3, 0x0
    li r4, 0x53
    bl fn_801F2838
    lis r3, lbl_80378A7C@ha
    addi r4, r3, lbl_80378A7C@l
    b @L_80231EE4
@L_80231ED0:
    li r3, 0x0
    li r4, 0x53
    bl fn_801F2A30
    lis r3, lbl_80378A8E@ha
    addi r4, r3, lbl_80378A8E@l
@L_80231EE4:
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
@L_80231EF0:
    bl fn_801DA7AC
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80231FB4
    cmplwi r30, 0x4
    bne @L_80231FA8
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x52
    bl fn_801F28E0
    mr r30, r3
    li r3, 0x0
    li r4, 0x52
    bl fn_801F288C
    extsb r3, r3
    extsb r0, r30
    addi r3, r3, 0x1
    extsb r5, r3
    cmpw r5, r0
    bge @L_80231F6C
    li r3, 0x0
    li r4, 0x52
    bl fn_801F2838
    lis r3, lbl_80378968@ha
    addi r4, r3, lbl_80378968@l
    b @L_80231F80
@L_80231F6C:
    li r3, 0x0
    li r4, 0x52
    bl fn_801F2A30
    lis r3, lbl_80378A4D@ha
    addi r4, r3, lbl_80378A4D@l
@L_80231F80:
    lis r3, lbl_80379F58@ha
    li r0, 0x1
    addi r3, r3, lbl_80379F58@l
    li r6, 0xd
    addis r5, r3, 0x1
    stb r0, 0x5(r31)
    lwz r3, lbl_8047B62C(r13)
    stb r6, 0x60a4(r5)
    li r5, 0x0
    bl fn_80211B94
@L_80231FA8:
    bl fn_801DA7AC
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
@L_80231FB4:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
