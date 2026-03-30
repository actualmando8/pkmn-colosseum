    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stmw r25, 0xf4(r1)
    mr r29, r4
    mr r31, r3
    mr r3, r29
    bl fn_8020E204
    bl fn_8020E1A4
    rlwinm r30, r3, 0, 24, 31
    mr r3, r31
    bl fn_80234A0C
    li r27, 0x0
    b @L_80234334
@L_80234304:
    mr r3, r31
    mr r4, r27
    bl fn_801F981C
    mr. r26, r3
    beq @L_80234330
    li r4, 0x1
    bl fn_80205C24
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80234330
    mr r3, r26
    bl fn_80207760
@L_80234330:
    addi r27, r27, 0x1
@L_80234334:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r30
    blt @L_80234304
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 24, 31
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r27, r0
    ble @L_8023442C
    lis r3, 0x1
    subi r3, r3, 0x13fc
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_802343D4
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_802343D4:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234424
    addi r3, r1, 0x94
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x94(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_80234424
    lis r3, 0x1
    subi r3, r3, 0x1399
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234424
    bl fn_8026246C
@L_80234424:
    li r0, 0x1
    b @L_802344B4
@L_8023442C:
    lis r3, 0x1
    subi r3, r3, 0x13fc
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_80234460
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_80234460:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802344B0
    addi r3, r1, 0x78
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x78(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_802344B0
    lis r3, 0x1
    subi r3, r3, 0x1398
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802344B0
    bl fn_8026246C
@L_802344B0:
    li r0, 0x0
@L_802344B4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234514
    li r26, 0x0
    b @L_80234508
@L_802344C8:
    mr r3, r31
    mr r4, r26
    bl fn_801F8518
    mr. r27, r3
    beq @L_80234504
    mr r4, r27
    li r3, 0x0
    li r5, 0x0
    bl fn_801F2020
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80234504
    mr r3, r31
    mr r4, r27
    mr r5, r29
    bl fn_8024E578
@L_80234504:
    addi r26, r26, 0x1
@L_80234508:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r30
    blt @L_802344C8
@L_80234514:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802346EC
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 24, 31
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r27, r0
    ble @L_80234620
    lis r3, 0x1
    subi r3, r3, 0x13ba
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_802345C8
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_802345C8:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234618
    addi r3, r1, 0x5c
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x5c(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_80234618
    lis r3, 0x1
    subi r3, r3, 0x1399
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234618
    bl fn_8026246C
@L_80234618:
    li r0, 0x1
    b @L_802346A8
@L_80234620:
    lis r3, 0x1
    subi r3, r3, 0x13ba
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_80234654
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_80234654:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802346A4
    addi r3, r1, 0x40
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x40(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_802346A4
    lis r3, 0x1
    subi r3, r3, 0x1398
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802346A4
    bl fn_8026246C
@L_802346A4:
    li r0, 0x0
@L_802346A8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802346EC
    li r26, 0x0
    b @L_802346E0
@L_802346BC:
    mr r3, r31
    mr r4, r26
    bl fn_801F8518
    mr. r4, r3
    beq @L_802346DC
    mr r3, r31
    mr r5, r29
    bl fn_802502EC
@L_802346DC:
    addi r26, r26, 0x1
@L_802346E0:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r30
    blt @L_802346BC
@L_802346EC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x25
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 24, 31
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r27, r0
    ble @L_802347D8
    lis r3, 0x1
    subi r3, r3, 0x13b9
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_80234780
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_80234780:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802347D0
    addi r3, r1, 0x24
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x24(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_802347D0
    lis r3, 0x1
    subi r3, r3, 0x1399
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802347D0
    bl fn_8026246C
@L_802347D0:
    li r0, 0x1
    b @L_80234860
@L_802347D8:
    lis r3, 0x1
    subi r3, r3, 0x13b9
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_8023480C
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_8023480C:
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023485C
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_8023485C
    lis r3, 0x1
    subi r3, r3, 0x1398
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023485C
    bl fn_8026246C
@L_8023485C:
    li r0, 0x0
@L_80234860:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802348A4
    li r26, 0x0
    b @L_80234898
@L_80234874:
    mr r3, r31
    mr r4, r26
    bl fn_801F8518
    mr. r4, r3
    beq @L_80234894
    mr r3, r31
    mr r5, r29
    bl fn_8024E534
@L_80234894:
    addi r26, r26, 0x1
@L_80234898:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r30
    blt @L_80234874
@L_802348A4:
    lis r3, 0x1
    subi r3, r3, 0x13d4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    cmplwi r31, 0x0
    beq @L_802348D8
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
@L_802348D8:
    li r3, 0x2f
    li r4, 0x64
    bl fn_80132A38
    bl fn_80008164
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234928
    addi r3, r1, 0xb0
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0xb0(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_80234928
    lis r3, 0x1
    subi r3, r3, 0x1399
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234928
    bl fn_8026246C
@L_80234928:
    li r5, 0x0
    addi r3, r1, 0xcc
    mr r4, r5
    b @L_80234944
@L_80234938:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
@L_80234944:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x8
    blt @L_80234938
    addi r28, r1, 0xcc
    li r25, 0x0
    li r26, 0x0
    b @L_80234994
@L_80234960:
    mr r3, r31
    mr r4, r26
    bl fn_801F8518
    mr. r27, r3
    beq @L_80234990
    li r4, 0x1
    bl fn_801FF1BC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80234990
    clrlslwi r0, r25, 16, 2
    addi r25, r25, 0x1
    stwx r27, r28, r0
@L_80234990:
    addi r26, r26, 0x1
@L_80234994:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r30
    blt @L_80234960
    rlwinm. r0, r25, 0, 16, 31
    beq @L_802349F8
    addi r4, r1, 0xcc
    li r3, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_801F3BB4
    addi r28, r1, 0xcc
    rlwinm r30, r25, 0, 16, 31
    li r27, 0x0
    b @L_802349EC
@L_802349CC:
    clrlslwi r0, r27, 16, 2
    lwzx r4, r28, r0
    cmplwi r4, 0x0
    beq @L_802349E8
    mr r3, r31
    mr r5, r29
    bl fn_8023A308
@L_802349E8:
    addi r27, r27, 0x1
@L_802349EC:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r30
    blt @L_802349CC
@L_802349F8:
    lmw r25, 0xf4(r1)
    lwz r0, 0x114(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
