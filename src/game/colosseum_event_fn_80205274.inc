    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r31, r3
    mr r27, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r29, 0x0
    b @L_802058E4
@L_802052D8:
    mr r3, r27
    mr r6, r29
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr r28, r3
    bl fn_801FA634
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802058E0
    li r30, 0x0
    b @L_802058D4
@L_80205304:
    mr r3, r28
    mr r6, r30
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r25, r3
    bne @L_80205328
    li r0, 0x0
    b @L_80205608
@L_80205328:
    bne @L_80205334
    li r0, 0x0
    b @L_80205450
@L_80205334:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_8020534C
    li r0, 0x0
    b @L_80205450
@L_8020534C:
    mr r3, r25
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r23, r3
    bne @L_80205370
    li r0, 0x0
    b @L_80205450
@L_80205370:
    bne @L_8020537C
    li r0, 0x0
    b @L_8020543C
@L_8020537C:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205394
    li r0, 0x0
    b @L_8020543C
@L_80205394:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802053B8
    li r0, 0x0
    b @L_8020543C
@L_802053B8:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802053CC
    li r0, 0x0
    b @L_8020543C
@L_802053CC:
    cmplwi r23, 0x0
    bne @L_802053DC
    li r3, 0x0
    b @L_802053F0
@L_802053DC:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802053F0:
    cmplwi r3, 0x0
    bne @L_80205400
    li r0, 0x0
    b @L_8020543C
@L_80205400:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205414
    li r0, 0x0
    b @L_8020543C
@L_80205414:
    mr r3, r23
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80205438
    li r0, 0x0
    b @L_8020543C
@L_80205438:
    li r0, 0x1
@L_8020543C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8020544C
    li r0, 0x0
    b @L_80205450
@L_8020544C:
    li r0, 0x1
@L_80205450:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205460
    li r0, 0x0
    b @L_80205608
@L_80205460:
    mr r3, r25
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80205484
    li r0, 0x0
    b @L_80205608
@L_80205484:
    mr r3, r25
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r23, r3
    bne @L_802054A8
    li r0, 0x0
    b @L_802055F4
@L_802054A8:
    bne @L_802054B4
    li r0, 0x0
    b @L_80205574
@L_802054B4:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_802054CC
    li r0, 0x0
    b @L_80205574
@L_802054CC:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802054F0
    li r0, 0x0
    b @L_80205574
@L_802054F0:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205504
    li r0, 0x0
    b @L_80205574
@L_80205504:
    cmplwi r23, 0x0
    bne @L_80205514
    li r3, 0x0
    b @L_80205528
@L_80205514:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80205528:
    cmplwi r3, 0x0
    bne @L_80205538
    li r0, 0x0
    b @L_80205574
@L_80205538:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020554C
    li r0, 0x0
    b @L_80205574
@L_8020554C:
    mr r3, r23
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80205570
    li r0, 0x0
    b @L_80205574
@L_80205570:
    li r0, 0x1
@L_80205574:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205584
    li r0, 0x0
    b @L_802055F4
@L_80205584:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_802055A8
    li r0, 0x0
    b @L_802055F4
@L_802055A8:
    cmplwi r23, 0x0
    bne @L_802055B8
    li r3, 0x0
    b @L_802055CC
@L_802055B8:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802055CC:
    cmplwi r3, 0x0
    bne @L_802055DC
    li r0, 0x0
    b @L_802055F4
@L_802055DC:
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802055F0
    li r0, 0x0
    b @L_802055F4
@L_802055F0:
    li r0, 0x1
@L_802055F4:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205604
    li r0, 0x0
    b @L_80205608
@L_80205604:
    li r0, 0x1
@L_80205608:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_802058D0
    mr r3, r25
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    cmplwi r31, 0x0
    mr r25, r3
    beq @L_802058D0
    cmplwi r25, 0x0
    bne @L_80205640
    li r0, 0x0
    b @L_80205700
@L_80205640:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205658
    li r0, 0x0
    b @L_80205700
@L_80205658:
    mr r3, r25
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_8020567C
    li r0, 0x0
    b @L_80205700
@L_8020567C:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205690
    li r0, 0x0
    b @L_80205700
@L_80205690:
    cmplwi r25, 0x0
    bne @L_802056A0
    li r3, 0x0
    b @L_802056B4
@L_802056A0:
    mr r3, r25
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802056B4:
    cmplwi r3, 0x0
    bne @L_802056C4
    li r0, 0x0
    b @L_80205700
@L_802056C4:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802056D8
    li r0, 0x0
    b @L_80205700
@L_802056D8:
    mr r3, r25
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802056FC
    li r0, 0x0
    b @L_80205700
@L_802056FC:
    li r0, 0x1
@L_80205700:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_802058D0
    cmplwi r31, 0x0
    bne @L_80205718
    li r0, 0x0
    b @L_80205858
@L_80205718:
    cmplwi r25, 0x0
    bne @L_80205728
    li r0, 0x0
    b @L_802057E8
@L_80205728:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205740
    li r0, 0x0
    b @L_802057E8
@L_80205740:
    mr r3, r25
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205764
    li r0, 0x0
    b @L_802057E8
@L_80205764:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205778
    li r0, 0x0
    b @L_802057E8
@L_80205778:
    cmplwi r25, 0x0
    bne @L_80205788
    li r3, 0x0
    b @L_8020579C
@L_80205788:
    mr r3, r25
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020579C:
    cmplwi r3, 0x0
    bne @L_802057AC
    li r0, 0x0
    b @L_802057E8
@L_802057AC:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802057C0
    li r0, 0x0
    b @L_802057E8
@L_802057C0:
    mr r3, r25
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802057E4
    li r0, 0x0
    b @L_802057E8
@L_802057E4:
    li r0, 0x1
@L_802057E8:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_802057F8
    li r0, 0x0
    b @L_80205858
@L_802057F8:
    mr r3, r25
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r22, r3
    li r23, 0x0
    b @L_80205848
@L_80205818:
    mr r3, r31
    rlwinm r6, r23, 0, 24, 31
    li r4, 0x0
    li r5, 0xfd
    bl fn_8012640C
    extsh. r0, r3
    blt @L_80205844
    cmpw r0, r22
    bne @L_80205844
    li r0, 0x1
    b @L_80205858
@L_80205844:
    addi r23, r23, 0x1
@L_80205848:
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0xc
    blt @L_80205818
    li r0, 0x0
@L_80205858:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802058D0
    mr r3, r25
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r25, r3
    li r23, 0x0
    b @L_802058C4
@L_80205884:
    rlwinm r22, r23, 0, 24, 31
    mr r3, r31
    mr r6, r22
    li r4, 0x0
    li r5, 0xfd
    bl fn_8012640C
    extsh. r0, r3
    bge @L_802058C0
    mr r3, r31
    mr r6, r22
    mr r7, r25
    li r4, 0x0
    li r5, 0xfd
    bl fn_801254B4
    b @L_802058D0
@L_802058C0:
    addi r23, r23, 0x1
@L_802058C4:
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0xc
    blt @L_80205884
@L_802058D0:
    addi r30, r30, 0x1
@L_802058D4:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r26
    blt @L_80205304
@L_802058E0:
    addi r29, r29, 0x1
@L_802058E4:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r24
    blt @L_802052D8
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
