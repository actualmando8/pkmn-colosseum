    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r22, r3
    li r31, 0x0
@L_801B9338:
    lbz r0, 0xd(r22)
    li r29, 0x0
    cmplwi r0, 0x0
    beq @L_801B9354
    lbz r0, 0xd(r22)
    cmplwi r0, 0x1
    bne @L_801B9920
@L_801B9354:
    lbz r0, 0x45(r22)
    cmplwi r0, 0x7
    bne @L_801B9920
    lbz r0, 0x34(r22)
    lwz r28, 0x38(r22)
    cmplwi r0, 0x1
    lbz r27, 0x35(r22)
    bne @L_801B9484
    lbz r0, 0x35(r22)
    cmplwi r0, 0x1
    bne @L_801B9484
    lbz r0, 0x11(r28)
    cmplwi r0, 0x0
    bne @L_801B9484
    lbz r0, 0x4d(r28)
    cmplwi r0, 0x7
    bne @L_801B9484
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801B93C8
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801B93C8
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801B93C8
    li r0, 0x1
    b @L_801B93FC
@L_801B93C8:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801B93F8
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801B93F8
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801B93F8
    li r0, 0x1
    b @L_801B93FC
@L_801B93F8:
    li r0, 0x0
@L_801B93FC:
    cmpwi r0, 0x0
    bne @L_801B9484
    lbz r0, 0x34(r22)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B9438
    lbz r0, 0x3c(r22)
    cmplwi r0, 0x4
    beq @L_801B9438
    lbz r0, 0x44(r22)
    cmplwi r0, 0x4
    beq @L_801B9438
    lbz r0, 0x4c(r22)
    cmplwi r0, 0x4
    bne @L_801B943C
@L_801B9438:
    li r3, 0x1
@L_801B943C:
    cmpwi r3, 0x0
    beq @L_801B9640
    lbz r0, 0x34(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B9478
    lbz r0, 0x3c(r28)
    cmplwi r0, 0x4
    beq @L_801B9478
    lbz r0, 0x44(r28)
    cmplwi r0, 0x4
    beq @L_801B9478
    lbz r0, 0x4c(r28)
    cmplwi r0, 0x4
    bne @L_801B947C
@L_801B9478:
    li r3, 0x1
@L_801B947C:
    cmpwi r3, 0x0
    beq @L_801B9640
@L_801B9484:
    lbz r0, 0xd(r22)
    lwz r28, 0x50(r22)
    cmplwi r0, 0x0
    lbz r27, 0x4d(r22)
    bne @L_801B9920
    lbz r0, 0x4c(r22)
    cmplwi r0, 0x1
    bne @L_801B9920
    lbz r0, 0x4d(r22)
    cmplwi r0, 0x1
    bne @L_801B9920
    lbz r0, 0x11(r28)
    cmplwi r0, 0x0
    bne @L_801B9920
    lbz r0, 0x4d(r28)
    cmplwi r0, 0x7
    bne @L_801B9920
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801B94F8
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801B94F8
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801B94F8
    li r0, 0x1
    b @L_801B952C
@L_801B94F8:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801B9528
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801B9528
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801B9528
    li r0, 0x1
    b @L_801B952C
@L_801B9528:
    li r0, 0x0
@L_801B952C:
    cmpwi r0, 0x0
    bne @L_801B9920
    lbz r0, 0x34(r22)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B9568
    lbz r0, 0x3c(r22)
    cmplwi r0, 0x4
    beq @L_801B9568
    lbz r0, 0x44(r22)
    cmplwi r0, 0x4
    beq @L_801B9568
    lbz r0, 0x4c(r22)
    cmplwi r0, 0x4
    bne @L_801B956C
@L_801B9568:
    li r3, 0x1
@L_801B956C:
    cmpwi r3, 0x0
    beq @L_801B95B4
    lbz r0, 0x34(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B95A8
    lbz r0, 0x3c(r28)
    cmplwi r0, 0x4
    beq @L_801B95A8
    lbz r0, 0x44(r28)
    cmplwi r0, 0x4
    beq @L_801B95A8
    lbz r0, 0x4c(r28)
    cmplwi r0, 0x4
    bne @L_801B95AC
@L_801B95A8:
    li r3, 0x1
@L_801B95AC:
    cmpwi r3, 0x0
    bne @L_801B9920
@L_801B95B4:
    lbz r0, 0x34(r22)
    cmplwi r0, 0x1
    bne @L_801B9610
    lbz r0, 0x35(r22)
    cmplwi r0, 0x1
    bne @L_801B95F0
    lwz r3, 0x38(r22)
    lbz r0, 0x11(r3)
    cmplwi r0, 0x0
    beq @L_801B9610
    lwz r3, 0x38(r22)
    lbz r0, 0xe(r3)
    cmplwi r0, 0x1
    bne @L_801B9920
    b @L_801B9610
@L_801B95F0:
    lwz r3, 0x38(r22)
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x0
    beq @L_801B9610
    lwz r3, 0x38(r22)
    lbz r0, 0x1a(r3)
    cmplwi r0, 0x1
    bne @L_801B9920
@L_801B9610:
    lwz r0, 0x34(r22)
    lwz r3, 0x38(r22)
    stw r0, 0x10(r1)
    lwz r0, 0x4c(r22)
    lwz r4, 0x50(r22)
    stw r3, 0x14(r1)
    lwz r3, 0x10(r1)
    stw r0, 0x34(r22)
    lwz r0, 0x14(r1)
    stw r4, 0x38(r22)
    stw r3, 0x4c(r22)
    stw r0, 0x50(r22)
@L_801B9640:
    lbz r0, 0x10(r28)
    cmplwi r0, 0x0
    bne @L_801B9710
    lbz r3, 0xf(r28)
    lbz r4, 0xf(r22)
    cmpwi r3, 0x2
    lbz r0, 0xd(r22)
    beq @L_801B9678
    bge @L_801B9680
    cmpwi r3, 0x1
    bge @L_801B9670
    b @L_801B9680
@L_801B9670:
    li r3, 0x1
    b @L_801B9684
@L_801B9678:
    li r3, -0x1
    b @L_801B9684
@L_801B9680:
    li r3, 0x0
@L_801B9684:
    cmpwi r0, 0x1
    bne @L_801B9690
    neg r3, r3
@L_801B9690:
    cmpwi r4, 0x2
    beq @L_801B96B0
    bge @L_801B96B4
    cmpwi r4, 0x1
    bge @L_801B96A8
    b @L_801B96B4
@L_801B96A8:
    addi r3, r3, 0x1
    b @L_801B96B4
@L_801B96B0:
    subi r3, r3, 0x1
@L_801B96B4:
    cmpwi r3, 0x0
    beq @L_801B96D8
    bge @L_801B96CC
    cmpwi r3, -0x1
    bge @L_801B96F0
    b @L_801B96FC
@L_801B96CC:
    cmpwi r3, 0x2
    bge @L_801B96FC
    b @L_801B96E4
@L_801B96D8:
    li r26, 0x0
    li r0, 0x1
    b @L_801B9700
@L_801B96E4:
    li r26, 0x1
    li r0, 0x1
    b @L_801B9700
@L_801B96F0:
    li r26, 0x2
    li r0, 0x1
    b @L_801B9700
@L_801B96FC:
    li r0, 0x0
@L_801B9700:
    cmpwi r0, 0x0
    beq @L_801B9920
    lbz r25, 0x10(r22)
    b @L_801B9840
@L_801B9710:
    lbz r0, 0x4d(r22)
    cmplwi r0, 0x7
    bne @L_801B9920
    lbz r0, 0xf(r22)
    cmplwi r0, 0x0
    bne @L_801B9920
    lbz r0, 0x10(r22)
    lbz r3, 0x10(r28)
    cmpwi r0, 0x2
    beq @L_801B9768
    bge @L_801B974C
    cmpwi r0, 0x0
    beq @L_801B9758
    bge @L_801B9760
    b @L_801B9778
@L_801B974C:
    cmpwi r0, 0x4
    bge @L_801B9778
    b @L_801B9770
@L_801B9758:
    li r0, 0x2
    b @L_801B977C
@L_801B9760:
    li r0, 0x4
    b @L_801B977C
@L_801B9768:
    li r0, 0x8
    b @L_801B977C
@L_801B9770:
    li r0, 0x1
    b @L_801B977C
@L_801B9778:
    li r0, 0x2
@L_801B977C:
    cmpwi r3, 0x2
    beq @L_801B97B4
    bge @L_801B9798
    cmpwi r3, 0x0
    beq @L_801B97A4
    bge @L_801B97AC
    b @L_801B97C4
@L_801B9798:
    cmpwi r3, 0x4
    bge @L_801B97C4
    b @L_801B97BC
@L_801B97A4:
    li r3, 0x2
    b @L_801B97C8
@L_801B97AC:
    li r3, 0x4
    b @L_801B97C8
@L_801B97B4:
    li r3, 0x8
    b @L_801B97C8
@L_801B97BC:
    li r3, 0x1
    b @L_801B97C8
@L_801B97C4:
    li r3, 0x2
@L_801B97C8:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801B9818
    bge @L_801B97FC
    cmpwi r0, 0x2
    beq @L_801B9810
    bge @L_801B9828
    cmpwi r0, 0x1
    bge @L_801B9808
    b @L_801B9828
@L_801B97FC:
    cmpwi r0, 0x8
    beq @L_801B9820
    b @L_801B9828
@L_801B9808:
    li r25, 0x3
    b @L_801B9830
@L_801B9810:
    li r25, 0x0
    b @L_801B9830
@L_801B9818:
    li r25, 0x1
    b @L_801B9830
@L_801B9820:
    li r25, 0x2
    b @L_801B9830
@L_801B9828:
    li r0, 0x0
    b @L_801B9834
@L_801B9830:
    li r0, 0x1
@L_801B9834:
    cmpwi r0, 0x0
    beq @L_801B9920
    lbz r26, 0xf(r28)
@L_801B9840:
    lbz r0, 0xd(r22)
    cmplwi r0, 0x1
    bne @L_801B9860
    lbz r0, 0xd(r28)
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    stb r0, 0xd(r22)
    b @L_801B9868
@L_801B9860:
    lbz r0, 0xd(r28)
    stb r0, 0xd(r22)
@L_801B9868:
    li r30, 0x0
    b @L_801B98B0
@L_801B9870:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    add r5, r28, r0
    rlwinm r3, r30, 3, 0, 28
    lwz r6, 0x34(r5)
    rlwinm r0, r30, 3, 0, 28
    lwz r5, 0x38(r5)
    add r7, r22, r4
    add r3, r22, r3
    add r4, r22, r0
    stw r6, 0x34(r7)
    stw r5, 0x38(r7)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    addi r30, r30, 0x1
@L_801B98B0:
    cmpwi r30, 0x3
    blt @L_801B9870
    stb r26, 0xf(r22)
    mr r3, r28
    mr r4, r27
    stb r25, 0x10(r22)
    bl fn_801B750C
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801B98E0
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801B98E0:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801B98F4
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801B98F4:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801B9908
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801B9908:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801B991C
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801B991C:
    li r29, 0x1
@L_801B9920:
    lbz r0, 0x4c(r22)
    lwz r28, 0x50(r22)
    cmplwi r0, 0x1
    lbz r27, 0x4d(r22)
    bne @L_801BA028
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801BA028
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801B9970
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801B9970
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801B9970
    li r0, 0x1
    b @L_801B99A4
@L_801B9970:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801B99A0
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801B99A0
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801B99A0
    li r0, 0x1
    b @L_801B99A4
@L_801B99A0:
    li r0, 0x0
@L_801B99A4:
    cmpwi r0, 0x0
    bne @L_801BA028
    lbz r0, 0x34(r22)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B99E0
    lbz r0, 0x3c(r22)
    cmplwi r0, 0x4
    beq @L_801B99E0
    lbz r0, 0x44(r22)
    cmplwi r0, 0x4
    beq @L_801B99E0
    lbz r0, 0x4c(r22)
    cmplwi r0, 0x4
    bne @L_801B99E4
@L_801B99E0:
    li r3, 0x1
@L_801B99E4:
    cmpwi r3, 0x0
    beq @L_801B9A2C
    lbz r0, 0x34(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801B9A20
    lbz r0, 0x3c(r28)
    cmplwi r0, 0x4
    beq @L_801B9A20
    lbz r0, 0x44(r28)
    cmplwi r0, 0x4
    beq @L_801B9A20
    lbz r0, 0x4c(r28)
    cmplwi r0, 0x4
    bne @L_801B9A24
@L_801B9A20:
    li r3, 0x1
@L_801B9A24:
    cmpwi r3, 0x0
    bne @L_801BA028
@L_801B9A2C:
    lbz r0, 0x11(r28)
    cmplwi r0, 0x0
    beq @L_801B9C74
    lbz r0, 0xe(r28)
    cmplwi r0, 0x1
    bne @L_801B9C74
    lbz r0, 0x35(r22)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x45(r22)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0xf(r22)
    cmplwi r0, 0x0
    bne @L_801BA028
    lbz r0, 0x10(r22)
    cmplwi r0, 0x3
    beq @L_801BA028
    lbz r0, 0x10(r22)
    lbz r3, 0x10(r28)
    cmpwi r0, 0x2
    beq @L_801B9AB4
    bge @L_801B9A98
    cmpwi r0, 0x0
    beq @L_801B9AA4
    bge @L_801B9AAC
    b @L_801B9AC4
@L_801B9A98:
    cmpwi r0, 0x4
    bge @L_801B9AC4
    b @L_801B9ABC
@L_801B9AA4:
    li r0, 0x2
    b @L_801B9AC8
@L_801B9AAC:
    li r0, 0x4
    b @L_801B9AC8
@L_801B9AB4:
    li r0, 0x8
    b @L_801B9AC8
@L_801B9ABC:
    li r0, 0x1
    b @L_801B9AC8
@L_801B9AC4:
    li r0, 0x2
@L_801B9AC8:
    cmpwi r3, 0x2
    beq @L_801B9B00
    bge @L_801B9AE4
    cmpwi r3, 0x0
    beq @L_801B9AF0
    bge @L_801B9AF8
    b @L_801B9B10
@L_801B9AE4:
    cmpwi r3, 0x4
    bge @L_801B9B10
    b @L_801B9B08
@L_801B9AF0:
    li r3, 0x2
    b @L_801B9B14
@L_801B9AF8:
    li r3, 0x4
    b @L_801B9B14
@L_801B9B00:
    li r3, 0x8
    b @L_801B9B14
@L_801B9B08:
    li r3, 0x1
    b @L_801B9B14
@L_801B9B10:
    li r3, 0x2
@L_801B9B14:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801B9B64
    bge @L_801B9B48
    cmpwi r0, 0x2
    beq @L_801B9B5C
    bge @L_801B9B74
    cmpwi r0, 0x1
    bge @L_801B9B54
    b @L_801B9B74
@L_801B9B48:
    cmpwi r0, 0x8
    beq @L_801B9B6C
    b @L_801B9B74
@L_801B9B54:
    li r25, 0x3
    b @L_801B9B7C
@L_801B9B5C:
    li r25, 0x0
    b @L_801B9B7C
@L_801B9B64:
    li r25, 0x1
    b @L_801B9B7C
@L_801B9B6C:
    li r25, 0x2
    b @L_801B9B7C
@L_801B9B74:
    li r0, 0x0
    b @L_801B9B80
@L_801B9B7C:
    li r0, 0x1
@L_801B9B80:
    cmpwi r0, 0x0
    beq @L_801BA028
    lbz r0, 0xf(r28)
    li r29, 0x1
    stb r0, 0xf(r22)
    stb r25, 0x10(r22)
    lbz r0, 0xe(r22)
    cmplwi r0, 0x1
    beq @L_801B9BAC
    lbz r0, 0xe(r28)
    stb r0, 0xe(r22)
@L_801B9BAC:
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801B9BC0
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801B9BC0:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801B9BD4
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801B9BD4:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801B9BE8
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801B9BE8:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801B9BFC
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801B9BFC:
    li r30, 0x0
    b @L_801B9C68
@L_801B9C04:
    rlwinm r6, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r5, r28, r3
    add r6, r22, r6
    add r3, r22, r4
    lwz r27, 0x38(r6)
    lbz r24, 0x35(r3)
    add r6, r22, r0
    lwz r4, 0x34(r5)
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x38(r5)
    rlwinm r0, r30, 3, 0, 28
    add r3, r22, r3
    stw r4, 0x34(r6)
    add r4, r22, r0
    stw r5, 0x38(r6)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    mr r3, r27
    mr r4, r24
    bl fn_801B750C
    addi r30, r30, 0x1
@L_801B9C68:
    cmpwi r30, 0x4
    blt @L_801B9C04
    b @L_801BA028
@L_801B9C74:
    lbz r0, 0x19(r22)
    cmplwi r0, 0xff
    beq @L_801B9CD4
    lbz r0, 0x19(r22)
    cmplwi r0, 0xe
    beq @L_801B9CD4
    lbz r0, 0x19(r22)
    cmplwi r0, 0xf
    beq @L_801B9CD4
    lbz r0, 0x19(r22)
    cmplwi r0, 0x0
    beq @L_801B9CD4
    lbz r0, 0x19(r22)
    cmplwi r0, 0x1
    beq @L_801B9CD4
    lbz r0, 0x35(r28)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x3d(r28)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x45(r28)
    cmplwi r0, 0x7
    bne @L_801BA028
@L_801B9CD4:
    lbz r0, 0x10(r28)
    cmplwi r0, 0x0
    bne @L_801B9DC4
    lbz r0, 0x35(r22)
    cmplwi r0, 0x7
    bne @L_801B9CF8
    lbz r0, 0x45(r22)
    cmplwi r0, 0x7
    beq @L_801B9D10
@L_801B9CF8:
    lbz r0, 0x35(r28)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x45(r28)
    cmplwi r0, 0x7
    bne @L_801BA028
@L_801B9D10:
    lbz r3, 0xf(r28)
    lbz r0, 0xf(r22)
    cmpwi r3, 0x2
    beq @L_801B9D38
    bge @L_801B9D40
    cmpwi r3, 0x1
    bge @L_801B9D30
    b @L_801B9D40
@L_801B9D30:
    li r3, 0x1
    b @L_801B9D44
@L_801B9D38:
    li r3, -0x1
    b @L_801B9D44
@L_801B9D40:
    li r3, 0x0
@L_801B9D44:
    cmpwi r0, 0x2
    beq @L_801B9D64
    bge @L_801B9D68
    cmpwi r0, 0x1
    bge @L_801B9D5C
    b @L_801B9D68
@L_801B9D5C:
    addi r3, r3, 0x1
    b @L_801B9D68
@L_801B9D64:
    subi r3, r3, 0x1
@L_801B9D68:
    cmpwi r3, 0x0
    beq @L_801B9D8C
    bge @L_801B9D80
    cmpwi r3, -0x1
    bge @L_801B9DA4
    b @L_801B9DB0
@L_801B9D80:
    cmpwi r3, 0x2
    bge @L_801B9DB0
    b @L_801B9D98
@L_801B9D8C:
    li r26, 0x0
    li r0, 0x1
    b @L_801B9DB4
@L_801B9D98:
    li r26, 0x1
    li r0, 0x1
    b @L_801B9DB4
@L_801B9DA4:
    li r26, 0x2
    li r0, 0x1
    b @L_801B9DB4
@L_801B9DB0:
    li r0, 0x0
@L_801B9DB4:
    cmpwi r0, 0x0
    beq @L_801BA028
    lbz r25, 0x10(r22)
    b @L_801B9F0C
@L_801B9DC4:
    lbz r0, 0x35(r22)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x3d(r22)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0x45(r22)
    cmplwi r0, 0x7
    bne @L_801BA028
    lbz r0, 0xf(r22)
    cmplwi r0, 0x0
    bne @L_801BA028
    lbz r0, 0x10(r22)
    lbz r3, 0x10(r28)
    cmpwi r0, 0x2
    beq @L_801B9E34
    bge @L_801B9E18
    cmpwi r0, 0x0
    beq @L_801B9E24
    bge @L_801B9E2C
    b @L_801B9E44
@L_801B9E18:
    cmpwi r0, 0x4
    bge @L_801B9E44
    b @L_801B9E3C
@L_801B9E24:
    li r0, 0x2
    b @L_801B9E48
@L_801B9E2C:
    li r0, 0x4
    b @L_801B9E48
@L_801B9E34:
    li r0, 0x8
    b @L_801B9E48
@L_801B9E3C:
    li r0, 0x1
    b @L_801B9E48
@L_801B9E44:
    li r0, 0x2
@L_801B9E48:
    cmpwi r3, 0x2
    beq @L_801B9E80
    bge @L_801B9E64
    cmpwi r3, 0x0
    beq @L_801B9E70
    bge @L_801B9E78
    b @L_801B9E90
@L_801B9E64:
    cmpwi r3, 0x4
    bge @L_801B9E90
    b @L_801B9E88
@L_801B9E70:
    li r3, 0x2
    b @L_801B9E94
@L_801B9E78:
    li r3, 0x4
    b @L_801B9E94
@L_801B9E80:
    li r3, 0x8
    b @L_801B9E94
@L_801B9E88:
    li r3, 0x1
    b @L_801B9E94
@L_801B9E90:
    li r3, 0x2
@L_801B9E94:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801B9EE4
    bge @L_801B9EC8
    cmpwi r0, 0x2
    beq @L_801B9EDC
    bge @L_801B9EF4
    cmpwi r0, 0x1
    bge @L_801B9ED4
    b @L_801B9EF4
@L_801B9EC8:
    cmpwi r0, 0x8
    beq @L_801B9EEC
    b @L_801B9EF4
@L_801B9ED4:
    li r25, 0x3
    b @L_801B9EFC
@L_801B9EDC:
    li r25, 0x0
    b @L_801B9EFC
@L_801B9EE4:
    li r25, 0x1
    b @L_801B9EFC
@L_801B9EEC:
    li r25, 0x2
    b @L_801B9EFC
@L_801B9EF4:
    li r0, 0x0
    b @L_801B9F00
@L_801B9EFC:
    li r0, 0x1
@L_801B9F00:
    cmpwi r0, 0x0
    beq @L_801BA028
    lbz r26, 0xf(r28)
@L_801B9F0C:
    lwz r3, 0x4c(r28)
    li r29, 0x1
    lwz r0, 0x50(r28)
    stw r3, 0x4c(r22)
    stw r0, 0x50(r22)
    lwz r3, 0x50(r22)
    lbz r4, 0x4d(r22)
    bl fn_801B7BD4
    stb r26, 0xf(r22)
    stb r25, 0x10(r22)
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801B9F48
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801B9F48:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801B9F5C
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801B9F5C:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801B9F70
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801B9F70:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801B9F84
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801B9F84:
    lbz r0, 0x35(r28)
    cmplwi r0, 0x7
    bne @L_801B9FA8
    lbz r0, 0x3d(r28)
    cmplwi r0, 0x7
    bne @L_801B9FA8
    lbz r0, 0x45(r28)
    cmplwi r0, 0x7
    beq @L_801BA01C
@L_801B9FA8:
    li r30, 0x0
    b @L_801BA014
@L_801B9FB0:
    rlwinm r6, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r5, r28, r3
    add r6, r22, r6
    add r3, r22, r4
    lwz r24, 0x38(r6)
    lbz r23, 0x35(r3)
    add r6, r22, r0
    lwz r4, 0x34(r5)
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x38(r5)
    rlwinm r0, r30, 3, 0, 28
    add r3, r22, r3
    stw r4, 0x34(r6)
    add r4, r22, r0
    stw r5, 0x38(r6)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    mr r3, r24
    mr r4, r23
    bl fn_801B750C
    addi r30, r30, 0x1
@L_801BA014:
    cmpwi r30, 0x3
    blt @L_801B9FB0
@L_801BA01C:
    mr r3, r28
    mr r4, r27
    bl fn_801B750C
@L_801BA028:
    lbz r0, 0x19(r22)
    cmplwi r0, 0x0
    beq @L_801BA040
    lbz r0, 0x19(r22)
    cmplwi r0, 0x1
    bne @L_801BA5C4
@L_801BA040:
    lbz r0, 0x65(r22)
    cmplwi r0, 0x7
    bne @L_801BA5C4
    lbz r0, 0x54(r22)
    lwz r28, 0x58(r22)
    cmplwi r0, 0x1
    lbz r27, 0x55(r22)
    bne @L_801BA164
    lbz r0, 0x1d(r28)
    cmplwi r0, 0x0
    bne @L_801BA164
    lbz r0, 0x6d(r28)
    cmplwi r0, 0x7
    bne @L_801BA164
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801BA0A8
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801BA0A8
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801BA0A8
    li r0, 0x1
    b @L_801BA0DC
@L_801BA0A8:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801BA0D8
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801BA0D8
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801BA0D8
    li r0, 0x1
    b @L_801BA0DC
@L_801BA0D8:
    li r0, 0x0
@L_801BA0DC:
    cmpwi r0, 0x0
    bne @L_801BA164
    lbz r0, 0x54(r22)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA118
    lbz r0, 0x5c(r22)
    cmplwi r0, 0x4
    beq @L_801BA118
    lbz r0, 0x64(r22)
    cmplwi r0, 0x4
    beq @L_801BA118
    lbz r0, 0x6c(r22)
    cmplwi r0, 0x4
    bne @L_801BA11C
@L_801BA118:
    li r3, 0x1
@L_801BA11C:
    cmpwi r3, 0x0
    beq @L_801BA2E4
    lbz r0, 0x54(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA158
    lbz r0, 0x5c(r28)
    cmplwi r0, 0x4
    beq @L_801BA158
    lbz r0, 0x64(r28)
    cmplwi r0, 0x4
    beq @L_801BA158
    lbz r0, 0x6c(r28)
    cmplwi r0, 0x4
    bne @L_801BA15C
@L_801BA158:
    li r3, 0x1
@L_801BA15C:
    cmpwi r3, 0x0
    beq @L_801BA2E4
@L_801BA164:
    lbz r0, 0x19(r22)
    lwz r28, 0x70(r22)
    cmplwi r0, 0x0
    lbz r27, 0x6d(r22)
    bne @L_801BA5C4
    lbz r0, 0x6c(r22)
    cmplwi r0, 0x1
    bne @L_801BA5C4
    lbz r0, 0x1d(r28)
    cmplwi r0, 0x0
    bne @L_801BA5C4
    lbz r0, 0x6d(r28)
    cmplwi r0, 0x7
    bne @L_801BA5C4
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801BA1CC
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801BA1CC
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801BA1CC
    li r0, 0x1
    b @L_801BA200
@L_801BA1CC:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801BA1FC
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801BA1FC
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801BA1FC
    li r0, 0x1
    b @L_801BA200
@L_801BA1FC:
    li r0, 0x0
@L_801BA200:
    cmpwi r0, 0x0
    bne @L_801BA5C4
    lbz r0, 0x54(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA23C
    lbz r0, 0x5c(r28)
    cmplwi r0, 0x4
    beq @L_801BA23C
    lbz r0, 0x64(r28)
    cmplwi r0, 0x4
    beq @L_801BA23C
    lbz r0, 0x6c(r28)
    cmplwi r0, 0x4
    bne @L_801BA240
@L_801BA23C:
    li r3, 0x1
@L_801BA240:
    cmpwi r3, 0x0
    beq @L_801BA288
    lbz r0, 0x54(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA27C
    lbz r0, 0x5c(r28)
    cmplwi r0, 0x4
    beq @L_801BA27C
    lbz r0, 0x64(r28)
    cmplwi r0, 0x4
    beq @L_801BA27C
    lbz r0, 0x6c(r28)
    cmplwi r0, 0x4
    bne @L_801BA280
@L_801BA27C:
    li r3, 0x1
@L_801BA280:
    cmpwi r3, 0x0
    bne @L_801BA5C4
@L_801BA288:
    lbz r0, 0x54(r22)
    cmplwi r0, 0x1
    bne @L_801BA2B4
    lwz r3, 0x58(r22)
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x0
    beq @L_801BA2B4
    lwz r3, 0x58(r22)
    lbz r0, 0x1a(r3)
    cmplwi r0, 0x1
    bne @L_801BA5C4
@L_801BA2B4:
    lwz r0, 0x54(r22)
    lwz r3, 0x58(r22)
    stw r0, 0x8(r1)
    lwz r0, 0x6c(r22)
    lwz r4, 0x70(r22)
    stw r3, 0xc(r1)
    lwz r3, 0x8(r1)
    stw r0, 0x54(r22)
    lwz r0, 0xc(r1)
    stw r4, 0x58(r22)
    stw r3, 0x6c(r22)
    stw r0, 0x70(r22)
@L_801BA2E4:
    lbz r0, 0x1c(r28)
    cmplwi r0, 0x0
    bne @L_801BA3B4
    lbz r3, 0x1b(r28)
    lbz r4, 0x1b(r22)
    cmpwi r3, 0x2
    lbz r0, 0x19(r22)
    beq @L_801BA31C
    bge @L_801BA324
    cmpwi r3, 0x1
    bge @L_801BA314
    b @L_801BA324
@L_801BA314:
    li r3, 0x1
    b @L_801BA328
@L_801BA31C:
    li r3, -0x1
    b @L_801BA328
@L_801BA324:
    li r3, 0x0
@L_801BA328:
    cmpwi r0, 0x1
    bne @L_801BA334
    neg r3, r3
@L_801BA334:
    cmpwi r4, 0x2
    beq @L_801BA354
    bge @L_801BA358
    cmpwi r4, 0x1
    bge @L_801BA34C
    b @L_801BA358
@L_801BA34C:
    addi r3, r3, 0x1
    b @L_801BA358
@L_801BA354:
    subi r3, r3, 0x1
@L_801BA358:
    cmpwi r3, 0x0
    beq @L_801BA37C
    bge @L_801BA370
    cmpwi r3, -0x1
    bge @L_801BA394
    b @L_801BA3A0
@L_801BA370:
    cmpwi r3, 0x2
    bge @L_801BA3A0
    b @L_801BA388
@L_801BA37C:
    li r26, 0x0
    li r0, 0x1
    b @L_801BA3A4
@L_801BA388:
    li r26, 0x1
    li r0, 0x1
    b @L_801BA3A4
@L_801BA394:
    li r26, 0x2
    li r0, 0x1
    b @L_801BA3A4
@L_801BA3A0:
    li r0, 0x0
@L_801BA3A4:
    cmpwi r0, 0x0
    beq @L_801BA5C4
    lbz r25, 0x1c(r22)
    b @L_801BA4E4
@L_801BA3B4:
    lbz r0, 0x6d(r22)
    cmplwi r0, 0x7
    bne @L_801BA5C4
    lbz r0, 0x1b(r22)
    cmplwi r0, 0x0
    bne @L_801BA5C4
    lbz r0, 0x1c(r22)
    lbz r3, 0x1c(r28)
    cmpwi r0, 0x2
    beq @L_801BA40C
    bge @L_801BA3F0
    cmpwi r0, 0x0
    beq @L_801BA3FC
    bge @L_801BA404
    b @L_801BA41C
@L_801BA3F0:
    cmpwi r0, 0x4
    bge @L_801BA41C
    b @L_801BA414
@L_801BA3FC:
    li r0, 0x2
    b @L_801BA420
@L_801BA404:
    li r0, 0x4
    b @L_801BA420
@L_801BA40C:
    li r0, 0x8
    b @L_801BA420
@L_801BA414:
    li r0, 0x1
    b @L_801BA420
@L_801BA41C:
    li r0, 0x2
@L_801BA420:
    cmpwi r3, 0x2
    beq @L_801BA458
    bge @L_801BA43C
    cmpwi r3, 0x0
    beq @L_801BA448
    bge @L_801BA450
    b @L_801BA468
@L_801BA43C:
    cmpwi r3, 0x4
    bge @L_801BA468
    b @L_801BA460
@L_801BA448:
    li r3, 0x2
    b @L_801BA46C
@L_801BA450:
    li r3, 0x4
    b @L_801BA46C
@L_801BA458:
    li r3, 0x8
    b @L_801BA46C
@L_801BA460:
    li r3, 0x1
    b @L_801BA46C
@L_801BA468:
    li r3, 0x2
@L_801BA46C:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801BA4BC
    bge @L_801BA4A0
    cmpwi r0, 0x2
    beq @L_801BA4B4
    bge @L_801BA4CC
    cmpwi r0, 0x1
    bge @L_801BA4AC
    b @L_801BA4CC
@L_801BA4A0:
    cmpwi r0, 0x8
    beq @L_801BA4C4
    b @L_801BA4CC
@L_801BA4AC:
    li r25, 0x3
    b @L_801BA4D4
@L_801BA4B4:
    li r25, 0x0
    b @L_801BA4D4
@L_801BA4BC:
    li r25, 0x1
    b @L_801BA4D4
@L_801BA4C4:
    li r25, 0x2
    b @L_801BA4D4
@L_801BA4CC:
    li r0, 0x0
    b @L_801BA4D8
@L_801BA4D4:
    li r0, 0x1
@L_801BA4D8:
    cmpwi r0, 0x0
    beq @L_801BA5C4
    lbz r26, 0x1b(r28)
@L_801BA4E4:
    lbz r0, 0x19(r22)
    cmplwi r0, 0x1
    bne @L_801BA504
    lbz r0, 0x19(r28)
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    stb r0, 0x19(r22)
    b @L_801BA50C
@L_801BA504:
    lbz r0, 0x19(r28)
    stb r0, 0x19(r22)
@L_801BA50C:
    li r30, 0x0
    b @L_801BA554
@L_801BA514:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    add r5, r28, r0
    rlwinm r3, r30, 3, 0, 28
    lwz r6, 0x54(r5)
    rlwinm r0, r30, 3, 0, 28
    lwz r5, 0x58(r5)
    add r7, r22, r4
    add r3, r22, r3
    add r4, r22, r0
    stw r6, 0x54(r7)
    stw r5, 0x58(r7)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    addi r30, r30, 0x1
@L_801BA554:
    cmpwi r30, 0x3
    blt @L_801BA514
    stb r26, 0x1b(r22)
    mr r3, r28
    mr r4, r27
    stb r25, 0x1c(r22)
    bl fn_801B750C
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801BA584
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801BA584:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801BA598
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801BA598:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801BA5AC
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801BA5AC:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801BA5C0
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801BA5C0:
    li r29, 0x1
@L_801BA5C4:
    lbz r0, 0x6c(r22)
    lwz r28, 0x70(r22)
    cmplwi r0, 0x1
    lbz r27, 0x6d(r22)
    bne @L_801BAC60
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    beq @L_801BA608
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801BA608
    lwz r3, 0x74(r22)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    beq @L_801BA608
    li r0, 0x1
    b @L_801BA63C
@L_801BA608:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    beq @L_801BA638
    lbz r0, 0x78(r28)
    cmplwi r0, 0xff
    beq @L_801BA638
    lbz r3, 0x78(r22)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    beq @L_801BA638
    li r0, 0x1
    b @L_801BA63C
@L_801BA638:
    li r0, 0x0
@L_801BA63C:
    cmpwi r0, 0x0
    bne @L_801BAC60
    lbz r0, 0x54(r22)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA678
    lbz r0, 0x5c(r22)
    cmplwi r0, 0x4
    beq @L_801BA678
    lbz r0, 0x64(r22)
    cmplwi r0, 0x4
    beq @L_801BA678
    lbz r0, 0x6c(r22)
    cmplwi r0, 0x4
    bne @L_801BA67C
@L_801BA678:
    li r3, 0x1
@L_801BA67C:
    cmpwi r3, 0x0
    beq @L_801BA6C4
    lbz r0, 0x54(r28)
    li r3, 0x0
    cmplwi r0, 0x4
    beq @L_801BA6B8
    lbz r0, 0x5c(r28)
    cmplwi r0, 0x4
    beq @L_801BA6B8
    lbz r0, 0x64(r28)
    cmplwi r0, 0x4
    beq @L_801BA6B8
    lbz r0, 0x6c(r28)
    cmplwi r0, 0x4
    bne @L_801BA6BC
@L_801BA6B8:
    li r3, 0x1
@L_801BA6BC:
    cmpwi r3, 0x0
    bne @L_801BAC60
@L_801BA6C4:
    lbz r0, 0x1d(r28)
    cmplwi r0, 0x0
    beq @L_801BA90C
    lbz r0, 0x1a(r28)
    cmplwi r0, 0x1
    bne @L_801BA90C
    lbz r0, 0x55(r22)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x65(r22)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x1b(r22)
    cmplwi r0, 0x0
    bne @L_801BAC60
    lbz r0, 0x1c(r22)
    cmplwi r0, 0x3
    beq @L_801BAC60
    lbz r0, 0x1c(r22)
    lbz r3, 0x1c(r28)
    cmpwi r0, 0x2
    beq @L_801BA74C
    bge @L_801BA730
    cmpwi r0, 0x0
    beq @L_801BA73C
    bge @L_801BA744
    b @L_801BA75C
@L_801BA730:
    cmpwi r0, 0x4
    bge @L_801BA75C
    b @L_801BA754
@L_801BA73C:
    li r0, 0x2
    b @L_801BA760
@L_801BA744:
    li r0, 0x4
    b @L_801BA760
@L_801BA74C:
    li r0, 0x8
    b @L_801BA760
@L_801BA754:
    li r0, 0x1
    b @L_801BA760
@L_801BA75C:
    li r0, 0x2
@L_801BA760:
    cmpwi r3, 0x2
    beq @L_801BA798
    bge @L_801BA77C
    cmpwi r3, 0x0
    beq @L_801BA788
    bge @L_801BA790
    b @L_801BA7A8
@L_801BA77C:
    cmpwi r3, 0x4
    bge @L_801BA7A8
    b @L_801BA7A0
@L_801BA788:
    li r3, 0x2
    b @L_801BA7AC
@L_801BA790:
    li r3, 0x4
    b @L_801BA7AC
@L_801BA798:
    li r3, 0x8
    b @L_801BA7AC
@L_801BA7A0:
    li r3, 0x1
    b @L_801BA7AC
@L_801BA7A8:
    li r3, 0x2
@L_801BA7AC:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801BA7FC
    bge @L_801BA7E0
    cmpwi r0, 0x2
    beq @L_801BA7F4
    bge @L_801BA80C
    cmpwi r0, 0x1
    bge @L_801BA7EC
    b @L_801BA80C
@L_801BA7E0:
    cmpwi r0, 0x8
    beq @L_801BA804
    b @L_801BA80C
@L_801BA7EC:
    li r25, 0x3
    b @L_801BA814
@L_801BA7F4:
    li r25, 0x0
    b @L_801BA814
@L_801BA7FC:
    li r25, 0x1
    b @L_801BA814
@L_801BA804:
    li r25, 0x2
    b @L_801BA814
@L_801BA80C:
    li r0, 0x0
    b @L_801BA818
@L_801BA814:
    li r0, 0x1
@L_801BA818:
    cmpwi r0, 0x0
    beq @L_801BAC60
    lbz r0, 0x1b(r28)
    li r29, 0x1
    stb r0, 0x1b(r22)
    stb r25, 0x1c(r22)
    lbz r0, 0x1a(r22)
    cmplwi r0, 0x1
    beq @L_801BA844
    lbz r0, 0x1a(r28)
    stb r0, 0x1a(r22)
@L_801BA844:
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801BA858
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801BA858:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801BA86C
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801BA86C:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801BA880
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801BA880:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801BA894
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801BA894:
    li r30, 0x0
    b @L_801BA900
@L_801BA89C:
    rlwinm r6, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r5, r28, r3
    add r6, r22, r6
    add r3, r22, r4
    lwz r23, 0x58(r6)
    lbz r24, 0x55(r3)
    add r6, r22, r0
    lwz r4, 0x54(r5)
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x58(r5)
    rlwinm r0, r30, 3, 0, 28
    add r3, r22, r3
    stw r4, 0x54(r6)
    add r4, r22, r0
    stw r5, 0x58(r6)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    mr r3, r23
    mr r4, r24
    bl fn_801B750C
    addi r30, r30, 0x1
@L_801BA900:
    cmpwi r30, 0x4
    blt @L_801BA89C
    b @L_801BAC60
@L_801BA90C:
    lbz r0, 0x1c(r28)
    cmplwi r0, 0x0
    bne @L_801BA9FC
    lbz r0, 0x55(r22)
    cmplwi r0, 0x7
    bne @L_801BA930
    lbz r0, 0x65(r22)
    cmplwi r0, 0x7
    beq @L_801BA948
@L_801BA930:
    lbz r0, 0x55(r28)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x65(r28)
    cmplwi r0, 0x7
    bne @L_801BAC60
@L_801BA948:
    lbz r3, 0x1b(r28)
    lbz r0, 0x1b(r22)
    cmpwi r3, 0x2
    beq @L_801BA970
    bge @L_801BA978
    cmpwi r3, 0x1
    bge @L_801BA968
    b @L_801BA978
@L_801BA968:
    li r3, 0x1
    b @L_801BA97C
@L_801BA970:
    li r3, -0x1
    b @L_801BA97C
@L_801BA978:
    li r3, 0x0
@L_801BA97C:
    cmpwi r0, 0x2
    beq @L_801BA99C
    bge @L_801BA9A0
    cmpwi r0, 0x1
    bge @L_801BA994
    b @L_801BA9A0
@L_801BA994:
    addi r3, r3, 0x1
    b @L_801BA9A0
@L_801BA99C:
    subi r3, r3, 0x1
@L_801BA9A0:
    cmpwi r3, 0x0
    beq @L_801BA9C4
    bge @L_801BA9B8
    cmpwi r3, -0x1
    bge @L_801BA9DC
    b @L_801BA9E8
@L_801BA9B8:
    cmpwi r3, 0x2
    bge @L_801BA9E8
    b @L_801BA9D0
@L_801BA9C4:
    li r26, 0x0
    li r0, 0x1
    b @L_801BA9EC
@L_801BA9D0:
    li r26, 0x1
    li r0, 0x1
    b @L_801BA9EC
@L_801BA9DC:
    li r26, 0x2
    li r0, 0x1
    b @L_801BA9EC
@L_801BA9E8:
    li r0, 0x0
@L_801BA9EC:
    cmpwi r0, 0x0
    beq @L_801BAC60
    lbz r25, 0x1c(r22)
    b @L_801BAB44
@L_801BA9FC:
    lbz r0, 0x55(r22)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x5d(r22)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x65(r22)
    cmplwi r0, 0x7
    bne @L_801BAC60
    lbz r0, 0x1b(r22)
    cmplwi r0, 0x0
    bne @L_801BAC60
    lbz r0, 0x1c(r22)
    lbz r3, 0x1c(r28)
    cmpwi r0, 0x2
    beq @L_801BAA6C
    bge @L_801BAA50
    cmpwi r0, 0x0
    beq @L_801BAA5C
    bge @L_801BAA64
    b @L_801BAA7C
@L_801BAA50:
    cmpwi r0, 0x4
    bge @L_801BAA7C
    b @L_801BAA74
@L_801BAA5C:
    li r0, 0x2
    b @L_801BAA80
@L_801BAA64:
    li r0, 0x4
    b @L_801BAA80
@L_801BAA6C:
    li r0, 0x8
    b @L_801BAA80
@L_801BAA74:
    li r0, 0x1
    b @L_801BAA80
@L_801BAA7C:
    li r0, 0x2
@L_801BAA80:
    cmpwi r3, 0x2
    beq @L_801BAAB8
    bge @L_801BAA9C
    cmpwi r3, 0x0
    beq @L_801BAAA8
    bge @L_801BAAB0
    b @L_801BAAC8
@L_801BAA9C:
    cmpwi r3, 0x4
    bge @L_801BAAC8
    b @L_801BAAC0
@L_801BAAA8:
    li r3, 0x2
    b @L_801BAACC
@L_801BAAB0:
    li r3, 0x4
    b @L_801BAACC
@L_801BAAB8:
    li r3, 0x8
    b @L_801BAACC
@L_801BAAC0:
    li r3, 0x1
    b @L_801BAACC
@L_801BAAC8:
    li r3, 0x2
@L_801BAACC:
    mullw r3, r0, r3
    rlwinm r0, r3, 1, 31, 31
    add r0, r0, r3
    srawi r0, r0, 1
    cmpwi r0, 0x4
    beq @L_801BAB1C
    bge @L_801BAB00
    cmpwi r0, 0x2
    beq @L_801BAB14
    bge @L_801BAB2C
    cmpwi r0, 0x1
    bge @L_801BAB0C
    b @L_801BAB2C
@L_801BAB00:
    cmpwi r0, 0x8
    beq @L_801BAB24
    b @L_801BAB2C
@L_801BAB0C:
    li r25, 0x3
    b @L_801BAB34
@L_801BAB14:
    li r25, 0x0
    b @L_801BAB34
@L_801BAB1C:
    li r25, 0x1
    b @L_801BAB34
@L_801BAB24:
    li r25, 0x2
    b @L_801BAB34
@L_801BAB2C:
    li r0, 0x0
    b @L_801BAB38
@L_801BAB34:
    li r0, 0x1
@L_801BAB38:
    cmpwi r0, 0x0
    beq @L_801BAC60
    lbz r26, 0x1b(r28)
@L_801BAB44:
    lwz r3, 0x6c(r28)
    li r29, 0x1
    lwz r0, 0x70(r28)
    stw r3, 0x6c(r22)
    stw r0, 0x70(r22)
    lwz r3, 0x70(r22)
    lbz r4, 0x6d(r22)
    bl fn_801B7BD4
    stb r26, 0x1b(r22)
    stb r25, 0x1c(r22)
    lwz r0, 0x74(r22)
    cmplwi r0, 0x0
    bne @L_801BAB80
    lwz r0, 0x74(r28)
    stw r0, 0x74(r22)
@L_801BAB80:
    lbz r0, 0x78(r22)
    cmplwi r0, 0xff
    bne @L_801BAB94
    lbz r0, 0x78(r28)
    stb r0, 0x78(r22)
@L_801BAB94:
    lbz r0, 0x1e(r22)
    cmplwi r0, 0xff
    bne @L_801BABA8
    lbz r0, 0x1e(r28)
    stb r0, 0x1e(r22)
@L_801BABA8:
    lbz r0, 0x1f(r22)
    cmplwi r0, 0xff
    bne @L_801BABBC
    lbz r0, 0x1f(r28)
    stb r0, 0x1f(r22)
@L_801BABBC:
    lbz r0, 0x55(r28)
    cmplwi r0, 0x7
    bne @L_801BABE0
    lbz r0, 0x5d(r28)
    cmplwi r0, 0x7
    bne @L_801BABE0
    lbz r0, 0x65(r28)
    cmplwi r0, 0x7
    beq @L_801BAC54
@L_801BABE0:
    li r30, 0x0
    b @L_801BAC4C
@L_801BABE8:
    rlwinm r6, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r5, r28, r3
    add r6, r22, r6
    add r3, r22, r4
    lwz r23, 0x58(r6)
    lbz r24, 0x55(r3)
    add r6, r22, r0
    lwz r4, 0x54(r5)
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x58(r5)
    rlwinm r0, r30, 3, 0, 28
    add r3, r22, r3
    stw r4, 0x54(r6)
    add r4, r22, r0
    stw r5, 0x58(r6)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    mr r3, r23
    mr r4, r24
    bl fn_801B750C
    addi r30, r30, 0x1
@L_801BAC4C:
    cmpwi r30, 0x3
    blt @L_801BABE8
@L_801BAC54:
    mr r3, r28
    mr r4, r27
    bl fn_801B750C
@L_801BAC60:
    cmpwi r29, 0x0
    beq @L_801BAC6C
    li r31, 0x1
@L_801BAC6C:
    cmpwi r29, 0x0
    bne @L_801B9338
    mr r3, r31
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
