    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r27, r4
    mr r31, r3
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80230594
    li r3, 0x0
    b @L_802316E8
@L_80230594:
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802305AC
    li r3, 0x1
    b @L_802316E8
@L_802305AC:
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r29, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80207BF4
    mr r28, r3
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230614
    li r3, 0x1
    b @L_802316E8
@L_80230614:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x25
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230688
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230688
    mr r3, r31
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r30
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_803790AB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803790AB@l
    li r5, 0x0
    bl fn_80211B94
@L_80230688:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802306B8
    li r3, 0x1
    b @L_802316E8
@L_802306B8:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_8022DF08
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230700
    li r3, 0x1
    b @L_802316E8
@L_80230700:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x0
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_8022BE2C
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023074C
    li r3, 0x1
    b @L_802316E8
@L_8023074C:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x1
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_8022BE2C
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230798
    li r3, 0x1
    b @L_802316E8
@L_80230798:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x1c
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023087C
    mr r3, r31
    li r4, 0x1c
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8023087C
    mr r4, r27
    bl fn_801F00D0
    mr. r24, r3
    beq @L_8023087C
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023087C
    mr r7, r24
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    li r4, 0x8
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r24
    mr r4, r27
    bl fn_801F0134
    lis r5, lbl_80379F58@ha
    mr r4, r27
    addi r5, r5, lbl_80379F58@l
    addis r5, r5, 0x1
    stb r3, 0x60a4(r5)
    mr r3, r31
    bl fn_801F0134
    lis r5, lbl_80379F58@ha
    lis r4, lbl_80378B72@ha
    addi r5, r5, lbl_80379F58@l
    addis r5, r5, 0x1
    addi r4, r4, lbl_80378B72@l
    stb r3, 0x60a5(r5)
    li r5, 0x0
    lwz r3, lbl_8047B62C(r13)
    bl fn_80211B94
@L_8023087C:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802308AC
    li r3, 0x1
    b @L_802316E8
@L_802308AC:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x3
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023090C
    mr r3, r31
    li r4, 0x8
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_8037925F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037925F@l
    li r5, 0x0
    bl fn_80211B94
@L_8023090C:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023093C
    li r3, 0x1
    b @L_802316E8
@L_8023093C:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x4
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802309EC
    mr r3, r31
    li r4, 0x4
    bl fn_80202360
    mr r26, r3
    mr r3, r31
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    extsh r0, r26
    mullw r7, r7, r0
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x4
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802309D8
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_802309D8:
    lis r4, lbl_8037925F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037925F@l
    li r5, 0x0
    bl fn_80211B94
@L_802309EC:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230A1C
    li r3, 0x1
    b @L_802316E8
@L_80230A1C:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x6
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230A7C
    mr r3, r31
    li r4, 0x8
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_80379287@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379287@l
    li r5, 0x0
    bl fn_80211B94
@L_80230A7C:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230AAC
    li r3, 0x1
    b @L_802316E8
@L_80230AAC:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x17
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230B34
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230B28
    mr r3, r31
    li r4, 0x4
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_80379402@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379402@l
    li r5, 0x0
    bl fn_80211B94
    b @L_80230B34
@L_80230B28:
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
@L_80230B34:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230B64
    li r3, 0x1
    b @L_802316E8
@L_80230B64:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x18
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230BC4
    mr r3, r31
    li r4, 0x4
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_8037941F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037941F@l
    li r5, 0x0
    bl fn_80211B94
@L_80230BC4:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230BF4
    li r3, 0x1
    b @L_802316E8
@L_80230BF4:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0xe
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230D04
    mr r3, r31
    li r4, 0xe
    bl fn_80202108
    mr r26, r3
    mr r3, r31
    li r4, 0xe
    bl fn_80201C58
    mr r0, r3
    li r3, 0x0
    mr r24, r0
    li r5, 0x1
    mr r4, r24
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    li r4, 0xe
    bl fn_80202234
    extsb r3, r3
    extsb r0, r26
    cmpw r0, r3
    blt @L_80230C9C
    mr r3, r31
    li r4, 0xe
    bl fn_80202810
    lis r3, lbl_803793C3@ha
    addi r0, r3, lbl_803793C3@l
    mr r4, r0
    b @L_80230CF8
@L_80230C9C:
    lis r3, lbl_80379F58@ha
    rlwinm r0, r24, 24, 24, 31
    addi r4, r3, lbl_80379F58@l
    addis r5, r4, 0x1
    mr r3, r31
    stb r24, 0x60a4(r5)
    li r4, 0x10
    stb r0, 0x60a5(r5)
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    addi r0, r26, 0x1
    mr r3, r31
    extsb r5, r0
    li r4, 0xe
    bl fn_80201FDC
    lis r3, lbl_803793A5@ha
    addi r0, r3, lbl_803793A5@l
    mr r4, r0
@L_80230CF8:
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
@L_80230D04:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230D34
    li r3, 0x1
    b @L_802316E8
@L_80230D34:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0xb
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230E50
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 5884
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    li r4, 0xb
    bl fn_80202108
    mr r26, r3
    mr r3, r31
    li r4, 0xb
    bl fn_80202234
    extsb r4, r26
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_80230DD0
    mr r3, r31
    li r4, 0xb
    bl fn_80202810
    b @L_80230DDC
@L_80230DD0:
    mr r3, r31
    li r4, 0xb
    bl fn_80201FDC
@L_80230DDC:
    mr r3, r31
    bl fn_80200A5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230E08
@L_80230DF0:
    mr r3, r31
    bl fn_80200B10
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_80230E3C
@L_80230E08:
    mr r3, r31
    li r4, 0xb
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230DF0
    mr r3, r31
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80230E3C:
    lis r4, lbl_8037930A@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037930A@l
    li r5, 0x0
    bl fn_80211B94
@L_80230E50:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230E80
    li r3, 0x1
    b @L_802316E8
@L_80230E80:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0xd
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230FCC
    mr r3, r31
    li r4, 0xd
    bl fn_80202108
    mr r26, r3
    mr r3, r31
    li r4, 0xd
    bl fn_80202234
    extsb r4, r26
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_80230EEC
    mr r3, r31
    li r4, 0xd
    bl fn_80202810
    b @L_80230EF8
@L_80230EEC:
    mr r3, r31
    li r4, 0xd
    bl fn_80201FDC
@L_80230EF8:
    mr r3, r31
    bl fn_80200A5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230F18
    mr r3, r31
    bl fn_80200B10
    b @L_80230FCC
@L_80230F18:
    mr r3, r31
    li r4, 0xd
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230FCC
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230FCC
    mr r3, r31
    li r4, 0x22
    bl fn_80202810
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230FCC
    lis r4, lbl_80399F58@ha
    lis r3, lbl_80379BFE@ha
    addi r26, r4, lbl_80399F58@l
    li r4, 0x47
    lwz r24, 0x1c(r26)
    addi r0, r3, lbl_80379BFE@l
    la r3, lbl_80478D78(r13)
    lwz r25, lbl_8047B610(r13)
    stb r4, 0x3(r3)
    li r3, 0x1
    li r4, 0x0
    stw r0, 0x1c(r26)
    bl fn_802249B8
    stw r25, lbl_8047B610(r13)
    mr r3, r31
    li r4, 0x9
    stw r24, 0x1c(r26)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230FCC
    lis r4, lbl_8037931E@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037931E@l
    li r5, 0x0
    bl fn_80211B94
@L_80230FCC:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230FFC
    li r3, 0x1
    b @L_802316E8
@L_80230FFC:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x29
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802310BC
    mr r3, r31
    li r4, 0x29
    bl fn_80202108
    mr r26, r3
    mr r3, r31
    li r4, 0x29
    bl fn_80201C58
    extsb r4, r26
    mr r25, r3
    addi r24, r4, 0x1
    mr r3, r31
    li r4, 0x29
    extsb r5, r24
    bl fn_80201FDC
    mr r3, r29
    mr r4, r25
    bl fn_80123B5C
    extsb. r0, r3
    bge @L_80231080
    mr r3, r31
    li r4, 0x29
    bl fn_80202810
    b @L_802310BC
@L_80231080:
    mr r3, r31
    li r4, 0x29
    bl fn_80202234
    extsb r3, r3
    extsb r0, r24
    cmpw r0, r3
    blt @L_802310BC
    mr r3, r31
    li r4, 0x29
    bl fn_80202810
    lis r4, lbl_80378D2C@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378D2C@l
    li r5, 0x0
    bl fn_80211B94
@L_802310BC:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802310EC
    li r3, 0x1
    b @L_802316E8
@L_802310EC:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x2a
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802311E8
    mr r3, r31
    li r4, 0x2a
    bl fn_80202108
    mr r26, r3
    mr r3, r31
    li r4, 0x2a
    bl fn_80201C58
    mr r30, r3
    mr r3, r31
    li r4, 0x2a
    bl fn_80201890
    mr r27, r3
    mr r3, r29
    rlwinm r6, r27, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    extsb r4, r26
    rlwinm r25, r3, 0, 16, 31
    addi r24, r4, 0x1
    mr r3, r31
    extsb r5, r24
    li r4, 0x2a
    bl fn_80201FDC
    rlwinm r0, r30, 0, 16, 31
    cmplw r25, r0
    beq @L_80231190
    mr r3, r31
    li r4, 0x2a
    bl fn_80202810
    b @L_802311E8
@L_80231190:
    mr r3, r31
    li r4, 0x2a
    bl fn_80202234
    extsb r3, r3
    extsb r0, r24
    cmpw r0, r3
    bge @L_802311C8
    mr r3, r29
    rlwinm r6, r27, 0, 16, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802311E8
@L_802311C8:
    mr r3, r31
    li r4, 0x2a
    bl fn_80202810
    lis r4, lbl_80378D40@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378D40@l
    li r5, 0x0
    bl fn_80211B94
@L_802311E8:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231218
    li r3, 0x1
    b @L_802316E8
@L_80231218:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x1d
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231290
    mr r3, r31
    li r4, 0x1d
    bl fn_80202108
    mr r27, r3
    mr r3, r31
    li r4, 0x1d
    bl fn_80202234
    extsb r4, r27
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_80231284
    mr r3, r31
    li r4, 0x1d
    bl fn_80202810
    b @L_80231290
@L_80231284:
    mr r3, r31
    li r4, 0x1d
    bl fn_80201FDC
@L_80231290:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802312C0
    li r3, 0x1
    b @L_802316E8
@L_802312C0:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x24
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231338
    mr r3, r31
    li r4, 0x24
    bl fn_80202108
    mr r27, r3
    mr r3, r31
    li r4, 0x24
    bl fn_80202234
    extsb r4, r27
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_8023132C
    mr r3, r31
    li r4, 0x24
    bl fn_80202810
    b @L_80231338
@L_8023132C:
    mr r3, r31
    li r4, 0x24
    bl fn_80201FDC
@L_80231338:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231368
    li r3, 0x1
    b @L_802316E8
@L_80231368:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x30
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802313E0
    mr r3, r31
    li r4, 0x30
    bl fn_80202108
    mr r27, r3
    mr r3, r31
    li r4, 0x30
    bl fn_80202234
    extsb r4, r27
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_802313D4
    mr r3, r31
    li r4, 0x30
    bl fn_80202810
    b @L_802313E0
@L_802313D4:
    mr r3, r31
    li r4, 0x30
    bl fn_80201FDC
@L_802313E0:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80231410
    li r3, 0x1
    b @L_802316E8
@L_80231410:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x26
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231680
    mr r3, r31
    li r4, 0x26
    bl fn_80202108
    mr r27, r3
    mr r3, r31
    li r4, 0x26
    bl fn_80202234
    extsb r4, r27
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_80231674
    mr r3, r31
    li r4, 0x26
    bl fn_80202810
    mr r3, r31
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231680
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x48
    beq @L_80231680
    cmplwi r0, 0xf
    beq @L_80231680
    lis r3, -32734
    mr r5, r31
    addi r4, r3, -14792
    li r6, 0x1
    li r3, 0x0
    bl fn_801F37B0
    rlwinm r0, r3, 0, 24, 31
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r0, r0, r3
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80231680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 24, 31
    bl fn_80077B60
    mr r27, r3
    bl fn_80077B3C
    cmplwi r26, 0x1
    bne @L_802315DC
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802315DC
    mr r4, r31
    li r29, 0x0
    li r3, 0x2
    bl fn_801F025C
    mr r25, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    li r27, 0x0
    b @L_802315BC
@L_80231550:
    mr r3, r25
    rlwinm r4, r27, 0, 16, 31
    bl fn_801F7258
    mr. r28, r3
    beq @L_802315B8
    li r26, 0x0
    b @L_802315AC
@L_8023156C:
    mr r3, r28
    mr r4, r26
    bl fn_801F986C
    mr. r24, r3
    beq @L_802315A8
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802315A8
    mr r3, r24
    li r4, 0x8
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802315A8
    addi r29, r29, 0x1
@L_802315A8:
    addi r26, r26, 0x1
@L_802315AC:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8023156C
@L_802315B8:
    addi r27, r27, 0x1
@L_802315BC:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r30
    blt @L_80231550
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_802315DC
    li r0, 0x1
    b @L_802315E0
@L_802315DC:
    li r0, 0x0
@L_802315E0:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80231680
    mr r3, r31
    li r4, 0x8
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80231680
    mr r3, r31
    li r4, 0x8
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    bl fn_80200B10
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80231644
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80231644:
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80379464@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379464@l
    li r5, 0x0
    bl fn_80211B94
    b @L_80231680
@L_80231674:
    mr r3, r31
    li r4, 0x26
    bl fn_80201FDC
@L_80231680:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802316B0
    li r3, 0x1
    b @L_802316E8
@L_802316B0:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x1
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_8022EDEC
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
    bl fn_801DA7AC
    li r3, 0x1
@L_802316E8:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
