    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    mr r3, r31
    mr r22, r0
    bl fn_80207BF4
    mr r25, r3
    mr r3, r31
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r5, 0x7
    mr r4, r29
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r23, r3, 0, 16, 31
    mr r4, r29
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    li r21, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r24, r31
    rlwinm r27, r3, 0, 16, 31
    la r30, lbl_80478D78(r13)
@L_8022F3B0:
    lwz r0, lbl_8047B644(r13)
    cmplwi r0, 0xf
    bgt @L_8022FDC0
    lis r3, jumptable_8039A538@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A538@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    li r4, 0x15
    bl fn_80202810
    mr r3, r31
    li r4, 0x28
    bl fn_80202810
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F558
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
    beq @L_8022F480
    mr r3, r31
    li r4, 0x8
    bl fn_80202810
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    li r0, 0x1
    lis r3, lbl_8037922A@ha
    addi r4, r3, lbl_8037922A@l
    stb r0, 0x5(r30)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
    li r21, 0x2
    b @L_8022F558
@L_8022F480:
    rlwinm r0, r25, 0, 16, 31
    mr r3, r31
    subfic r0, r0, 0x30
    li r4, 0x8
    cntlzw r0, r0
    rlwinm r5, r0, 27, 5, 31
    addi r20, r5, 0x1
    bl fn_80202108
    add r20, r3, r20
    mr r3, r31
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r20
    cmpw r0, r3
    blt @L_8022F4DC
    mr r3, r31
    li r4, 0x8
    bl fn_80202810
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    b @L_8022F4EC
@L_8022F4DC:
    mr r3, r31
    mr r5, r20
    li r4, 0x8
    bl fn_80201FDC
@L_8022F4EC:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F538
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0xad
    beq @L_8022F558
    cmplwi r0, 0xd6
    beq @L_8022F558
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_80379217@ha
    addi r0, r3, lbl_80379217@l
    li r21, 0x2
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
    b @L_8022F558
@L_8022F538:
    li r0, 0x0
    lis r3, lbl_8037922A@ha
    addi r4, r3, lbl_8037922A@l
    stb r0, 0x5(r30)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
    li r21, 0x2
@L_8022F558:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F5FC
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x5
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    beq @L_8022F5D0
    cmplwi r26, 0x7d
    beq @L_8022F5C0
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_803792A0@ha
    addi r0, r3, lbl_803792A0@l
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
    b @L_8022F5F8
@L_8022F5C0:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
@L_8022F5D0:
    mr r3, r31
    li r4, 0x7
    bl fn_80202810
    li r0, 0x0
    lis r3, lbl_803792B3@ha
    addi r4, r3, lbl_803792B3@l
    stb r0, 0x5(r30)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
@L_8022F5F8:
    li r21, 0x2
@L_8022F5FC:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x36
    bne @L_8022F688
    mr r3, r31
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022F688
    mr r3, r28
    li r4, 0x40
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022F65C
    mr r3, r28
    li r4, 0x40
    li r5, 0x0
    bl fn_802097C8
@L_8022F65C:
    mr r3, r31
    bl fn_80200B10
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_80379992@ha
    addi r0, r3, lbl_80379992@l
    li r3, 0x0
    oris r4, r4, 0x8
    stb r3, 0x5(r30)
    li r21, 0x1
    stw r4, lbl_8047B618(r13)
    stw r0, lbl_8047B610(r13)
@L_8022F688:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x12
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F6E0
    mr r3, r31
    li r4, 0x12
    bl fn_80202810
    mr r3, r31
    bl fn_80200B10
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_80376D00@ha
    addi r0, r3, lbl_80376D00@l
    li r21, 0x1
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
@L_8022F6E0:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x11
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F750
    mr r3, r31
    li r4, 0x11
    bl fn_80202810
    mr r3, r31
    li r4, 0x0
    li r5, 0x110
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r31
    bl fn_80200B10
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_803792F2@ha
    addi r0, r3, lbl_803792F2@l
    li r21, 0x1
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
@L_8022F750:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x29
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F7EC
    mr r3, r31
    li r4, 0x29
    bl fn_80201C58
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    bne @L_8022F7EC
    cmplwi r0, 0x0
    beq @L_8022F7EC
    cmplwi r0, 0x165
    beq @L_8022F7EC
    mr r3, r31
    bl fn_80200B10
    mr r3, r31
    li r4, 0x0
    li r5, 0x10d
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, lbl_8047B618(r13)
    mr r4, r31
    li r3, 0x11
    oris r0, r0, 0x8
    stw r0, lbl_8047B618(r13)
    bl fn_80132A38
    lis r3, lbl_80378D14@ha
    li r21, 0x1
    addi r0, r3, lbl_80378D14@l
    stw r0, lbl_8047B610(r13)
@L_8022F7EC:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x30
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F864
    cmplwi r23, 0x0
    bne @L_8022F864
    mr r3, r31
    bl fn_80200B10
    mr r3, r31
    li r4, 0x0
    li r5, 0x10e
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, lbl_8047B618(r13)
    mr r4, r31
    li r3, 0x11
    oris r0, r0, 0x8
    stw r0, lbl_8047B618(r13)
    bl fn_80132A38
    lis r3, lbl_8037903A@ha
    li r21, 0x1
    addi r0, r3, lbl_8037903A@l
    stw r0, lbl_8047B610(r13)
@L_8022F864:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r4, r31
    mr r5, r29
    li r3, 0x0
    bl fn_801F1F30
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022F8D8
    mr r3, r31
    bl fn_80200B10
    mr r3, r31
    li r4, 0x0
    li r5, 0x10b
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, lbl_8047B618(r13)
    mr r4, r31
    li r3, 0x11
    oris r0, r0, 0x8
    stw r0, lbl_8047B618(r13)
    bl fn_80132A38
    lis r3, lbl_8037914F@ha
    li r21, 0x1
    addi r0, r3, lbl_8037914F@l
    stw r0, lbl_8047B610(r13)
@L_8022F8D8:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FA1C
    mr r3, r31
    li r4, 0x9
    bl fn_80202108
    mr r21, r3
    mr r3, r31
    li r4, 0x9
    bl fn_80202234
    extsb r3, r3
    extsb r0, r21
    cmpw r0, r3
    blt @L_8022F950
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    lis r4, lbl_80379388@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379388@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022FA18
@L_8022F950:
    addi r0, r21, 0x1
    mr r3, r31
    extsb r5, r0
    li r4, 0x9
    bl fn_80201FDC
    bl fn_800E0C54
    rlwinm. r0, r3, 0, 31, 31
    beq @L_8022F990
    li r0, 0x0
    lis r3, lbl_80379332@ha
    addi r4, r3, lbl_80379332@l
    stb r0, 0x5(r30)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
    b @L_8022FA18
@L_8022F990:
    li r0, 0x1
    mr r7, r24
    stb r0, 0x5(r30)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    mr r4, r31
    mr r5, r22
    li r6, 0x1
    li r7, 0x28
    li r8, -0x1
    bl fn_80232110
    mr r0, r3
    mr r3, r28
    mr r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x107
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_80379332@ha
    addi r0, r3, lbl_80379332@l
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
@L_8022FA18:
    li r21, 0x1
@L_8022FA1C:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FAA0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r3, 30, 0, 1
    rlwinm r3, r4, 1, 31, 31
    subf r0, r3, r0
    rlwinm r0, r0, 2, 0, 31
    add. r0, r0, r3
    bne @L_8022FAA0
    mr r3, r31
    li r4, 0x0
    li r5, 0x106
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r31
    bl fn_80200B10
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_803792DF@ha
    addi r0, r3, lbl_803792DF@l
    li r21, 0x1
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
@L_8022FAA0:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FB6C
    mr r3, r31
    li r4, 0xa
    bl fn_80201D84
    mr r4, r27
    bl fn_801F00D0
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    bl fn_800E0C54
    rlwinm. r0, r3, 0, 31, 31
    beq @L_8022FB1C
    lis r4, lbl_803793D7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803793D7@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022FB68
@L_8022FB1C:
    mr r3, r31
    bl fn_80200B10
    mr r3, r31
    li r4, 0x0
    li r5, 0x10c
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_803793D7@ha
    addi r4, r3, lbl_803793D7@l
    lwz r3, lbl_8047B62C(r13)
    oris r0, r0, 0x8
    li r5, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
    lis r3, lbl_803793EA@ha
    addi r0, r3, lbl_803793EA@l
    stw r0, lbl_8047B610(r13)
@L_8022FB68:
    li r21, 0x1
@L_8022FB6C:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0xc
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FCE4
    mr r3, r31
    li r4, 0xc
    bl fn_80202108
    mr r21, r3
    mr r3, r31
    li r4, 0xc
    bl fn_80202234
    extsb r4, r21
    extsb r0, r3
    addi r3, r4, 0x1
    extsb r5, r3
    cmpw r5, r0
    blt @L_8022FCC8
    mr r3, r31
    li r4, 0xc
    bl fn_80202810
    mr r3, r31
    li r4, 0x22
    bl fn_80202810
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x0
    li r5, 0xf5
    oris r0, r0, 0x200
    li r6, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_8012640C
    extsh r21, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0xf6
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    mr r4, r27
    bl fn_801F00D0
    extsh. r0, r21
    mr r20, r3
    beq @L_8022FCB8
    mr r3, r28
    li r4, 0x75
    bl fn_80209C1C
    mr r3, r31
    bl fn_80205184
    rlwinm r0, r21, 1, 0, 30
    mr r29, r3
    stw r0, lbl_8047B620(r13)
    mr r3, r20
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022FC90
    mr r3, r31
    mr r4, r29
    mr r5, r27
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, 0x0
    bl fn_8022B2CC
    mr r20, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022FCB8
@L_8022FC90:
    mr r7, r20
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lis r3, lbl_80378C3B@ha
    addi r0, r3, lbl_80378C3B@l
    stw r0, lbl_8047B610(r13)
    b @L_8022FCE0
@L_8022FCB8:
    lis r3, lbl_80378CB6@ha
    addi r0, r3, lbl_80378CB6@l
    stw r0, lbl_8047B610(r13)
    b @L_8022FCE0
@L_8022FCC8:
    mr r3, r31
    li r4, 0xc
    bl fn_80201FDC
    lis r3, lbl_80378C23@ha
    addi r0, r3, lbl_80378C23@l
    stw r0, lbl_8047B610(r13)
@L_8022FCE0:
    li r21, 0x1
@L_8022FCE4:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FD40
    cmplwi r26, 0x7d
    bne @L_8022FD3C
    mr r3, r31
    li r4, 0x7
    bl fn_80202810
    li r0, 0x1
    lis r3, lbl_803792B3@ha
    addi r4, r3, lbl_803792B3@l
    stb r0, 0x5(r30)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
@L_8022FD3C:
    li r21, 0x2
@L_8022FD40:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
    b @L_8022FDC0
    mr r3, r31
    li r4, 0x3e
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022FD98
    mr r3, r31
    li r4, 0x0
    bl fn_8022EDEC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FDB4
    mr r3, r31
    bl fn_80200B10
    lis r3, lbl_80379212@ha
    li r21, 0x2
    addi r0, r3, lbl_80379212@l
    stw r0, lbl_8047B610(r13)
    b @L_8022FDB4
@L_8022FD98:
    mr r3, r31
    li r4, 0x2
    bl fn_8022EDEC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FDB4
    li r21, 0x2
@L_8022FDB4:
    lwz r3, lbl_8047B644(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B644(r13)
@L_8022FDC0:
    lwz r0, lbl_8047B644(r13)
    cmpwi r0, 0xf
    beq @L_8022FDD4
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022F3B0
@L_8022FDD4:
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022FE08
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022FE08
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022FE08:
    mr r3, r21
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
