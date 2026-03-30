    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r29, r3
    cmplwi r29, 0x0
    lis r4, -32729
    addi r25, r4, -3352
    bne @L_80084064
    li r3, 0xa6
    bl fn_80104704
@L_80084064:
    bl fn_801040A0
    lbz r0, 0x1(r29)
    lwz r26, 0x0(r3)
    extsb r0, r0
    cmpwi r0, 0x3
    mr r30, r26
    beq @L_800843D4
    bge @L_80084090
    cmpwi r0, 0x0
    beq @L_8008409C
    b @L_80084458
@L_80084090:
    cmpwi r0, 0x5
    beq @L_8008441C
    b @L_80084458
@L_8008409C:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80084458
    li r3, 0xe0
    li r4, 0x20
    bl fn_800E2C04
    mr r26, r3
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_800840D8
    addi r3, r25, 0x184
    li r4, 0xea
    addi r5, r2, -29960
    bl fn_80196E10
@L_800840D8:
    mr r3, r26
    bl fn_800E27B0
    li r4, 0x0
    mr r26, r3
    li r5, 0xcc
    bl memset
    mr r30, r26
    mr r3, r29
    bl fn_801040A0
    stw r26, 0x0(r3)
    li r5, 0x0
    li r0, 0x1
    mr r3, r29
    stw r5, 0x10(r26)
    li r4, 0x10f6
    stw r5, 0x0(r26)
    stw r5, 0x14(r26)
    stw r5, 0x4(r26)
    stw r5, 0x18(r26)
    stw r5, 0x8(r26)
    stw r5, 0x1c(r26)
    stw r5, 0xc(r26)
    stb r0, 0x20(r26)
    stb r0, 0x21(r26)
    stw r5, 0x24(r26)
    stw r5, 0x28(r26)
    bl fn_801046C8
    stw r3, 0x30(r26)
    mr r3, r29
    li r4, 0x10f7
    bl fn_801046C8
    stw r3, 0x34(r26)
    mr r3, r29
    li r4, 0x10d5
    bl fn_801046C8
    stw r3, 0x38(r26)
    mr r3, r29
    li r4, 0x10da
    bl fn_801046C8
    stw r3, 0x3c(r26)
    mr r3, r29
    li r4, 0x10e3
    bl fn_801046C8
    stw r3, 0x40(r26)
    mr r3, r29
    li r4, 0x10f0
    bl fn_801046C8
    stw r3, 0x44(r26)
    mr r3, r29
    li r4, 0x10f5
    bl fn_801046C8
    stw r3, 0x48(r26)
    mr r3, r29
    li r4, 0x10d1
    bl fn_801046C8
    stw r3, 0x4c(r26)
    mr r3, r29
    li r4, 0x10d6
    bl fn_801046C8
    stw r3, 0x5c(r26)
    mr r3, r29
    li r4, 0x10db
    bl fn_801046C8
    stw r3, 0x6c(r26)
    mr r3, r29
    li r4, 0x10df
    bl fn_801046C8
    stw r3, 0x7c(r26)
    mr r3, r29
    li r4, 0x10e4
    bl fn_801046C8
    stw r3, 0x8c(r26)
    mr r3, r29
    li r4, 0x10e8
    bl fn_801046C8
    stw r3, 0x9c(r26)
    mr r3, r29
    li r4, 0x10ec
    bl fn_801046C8
    stw r3, 0xac(r26)
    mr r3, r29
    li r4, 0x10f1
    bl fn_801046C8
    stw r3, 0xbc(r26)
    mr r3, r29
    li r4, 0x10d2
    bl fn_801046C8
    stw r3, 0x50(r26)
    mr r3, r29
    li r4, 0x10d7
    bl fn_801046C8
    stw r3, 0x60(r26)
    mr r3, r29
    li r4, 0x10dc
    bl fn_801046C8
    stw r3, 0x70(r26)
    mr r3, r29
    li r4, 0x10e0
    bl fn_801046C8
    stw r3, 0x80(r26)
    mr r3, r29
    li r4, 0x10e5
    bl fn_801046C8
    stw r3, 0x90(r26)
    mr r3, r29
    li r4, 0x10e9
    bl fn_801046C8
    stw r3, 0xa0(r26)
    mr r3, r29
    li r4, 0x10ed
    bl fn_801046C8
    stw r3, 0xb0(r26)
    mr r3, r29
    li r4, 0x10f2
    bl fn_801046C8
    stw r3, 0xc0(r26)
    mr r3, r29
    li r4, 0x10d3
    bl fn_801046C8
    stw r3, 0x54(r26)
    mr r3, r29
    li r4, 0x10d8
    bl fn_801046C8
    stw r3, 0x64(r26)
    mr r3, r29
    li r4, 0x10dd
    bl fn_801046C8
    stw r3, 0x74(r26)
    mr r3, r29
    li r4, 0x10e1
    bl fn_801046C8
    stw r3, 0x84(r26)
    mr r3, r29
    li r4, 0x10e6
    bl fn_801046C8
    stw r3, 0x94(r26)
    mr r3, r29
    li r4, 0x10ea
    bl fn_801046C8
    stw r3, 0xa4(r26)
    mr r3, r29
    li r4, 0x10ee
    bl fn_801046C8
    stw r3, 0xb4(r26)
    mr r3, r29
    li r4, 0x10f3
    bl fn_801046C8
    stw r3, 0xc4(r26)
    mr r3, r29
    li r4, 0x10d4
    bl fn_801046C8
    stw r3, 0x58(r26)
    mr r3, r29
    li r4, 0x10d9
    bl fn_801046C8
    stw r3, 0x68(r26)
    mr r3, r29
    li r4, 0x10de
    bl fn_801046C8
    stw r3, 0x78(r26)
    mr r3, r29
    li r4, 0x10e2
    bl fn_801046C8
    stw r3, 0x88(r26)
    mr r3, r29
    li r4, 0x10e7
    bl fn_801046C8
    stw r3, 0x98(r26)
    mr r3, r29
    li r4, 0x10eb
    bl fn_801046C8
    stw r3, 0xa8(r26)
    mr r3, r29
    li r4, 0x10ef
    bl fn_801046C8
    stw r3, 0xb8(r26)
    mr r3, r29
    li r4, 0x10f4
    bl fn_801046C8
    stw r3, 0xc8(r26)
    addi r24, r25, 0x70
    li r26, 0x0
@L_800843B0:
    lhz r4, 0x0(r24)
    mr r3, r29
    lhz r5, 0x2(r24)
    bl fn_801081F8
    addi r24, r24, 0x6
    addi r26, r26, 0x1
    cmplwi r26, 0x2e
    blt @L_800843B0
    b @L_80084458
@L_800843D4:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80084458
    addi r24, r25, 0x70
    li r27, 0x0
@L_800843EC:
    lhz r4, 0x0(r24)
    mr r3, r29
    lhz r5, 0x4(r24)
    bl fn_801081F8
    addi r24, r24, 0x6
    addi r27, r27, 0x1
    cmplwi r27, 0x2e
    blt @L_800843EC
    li r0, 0x1
    stb r0, 0x2(r29)
    stb r0, 0x20(r26)
    b @L_80084458
@L_8008441C:
    mr r3, r26
    bl fn_800E202C
    mr r26, r3
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80084444
    addi r3, r25, 0x184
    li r4, 0xf3
    addi r5, r2, -29960
    bl fn_80196E10
@L_80084444:
    mr r3, r26
    bl fn_800E24B0
    mr r3, r26
    bl fn_800E209C
    b @L_800849A0
@L_80084458:
    lbz r0, 0x1(r29)
    li r27, 0x0
    extsb r0, r0
    cmpwi r0, 0x2
    bne @L_80084484
    lbz r0, 0x20(r30)
    cmplwi r0, 0x0
    beq @L_80084484
    li r0, 0x0
    li r27, 0x1
    stb r0, 0x20(r30)
@L_80084484:
    mr r24, r30
    li r31, 0x0
    addi r23, r13, -32464
    li r28, 0x7
    li r26, 0x8
@L_80084498:
    lwz r0, 0x0(r24)
    cmpwi r0, 0x5
    beq @L_800844AC
    cmpwi r0, 0x4
    bne @L_800844DC
@L_800844AC:
    addi r3, r31, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800844DC
    bl fn_80103CB0
    lbz r0, 0x0(r23)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    stw r28, 0x0(r24)
    stw r26, 0x28(r30)
@L_800844DC:
    addi r24, r24, 0x4
    addi r23, r23, 0x1
    addi r31, r31, 0x1
    cmpwi r31, 0x3
    ble @L_80084498
    lwz r3, 0x10(r30)
    lwz r0, 0x0(r30)
    cmpw r3, r0
    beq @L_80084504
    li r27, 0x1
@L_80084504:
    lwz r0, 0x0(r30)
    stw r0, 0x10(r30)
    lwz r3, 0x14(r30)
    lwz r0, 0x4(r30)
    cmpw r3, r0
    beq @L_80084520
    li r27, 0x1
@L_80084520:
    lwz r0, 0x4(r30)
    stw r0, 0x14(r30)
    lwz r3, 0x18(r30)
    lwz r0, 0x8(r30)
    cmpw r3, r0
    beq @L_8008453C
    li r27, 0x1
@L_8008453C:
    lwz r0, 0x8(r30)
    stw r0, 0x18(r30)
    lwz r3, 0x1c(r30)
    lwz r0, 0xc(r30)
    cmpw r3, r0
    beq @L_80084558
    li r27, 0x1
@L_80084558:
    lwz r0, 0xc(r30)
    li r26, 0x4
    stw r0, 0x1c(r30)
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800849A0
    lwz r3, 0x30(r30)
    cmplwi r3, 0x0
    beq @L_80084584
    li r4, 0x1
    bl fn_80109220
@L_80084584:
    lwz r3, 0x34(r30)
    cmplwi r3, 0x0
    beq @L_80084598
    li r4, 0x1
    bl fn_80109220
@L_80084598:
    lwz r0, 0x0(r30)
    addi r4, r25, 0x0
    lwz r3, 0x38(r30)
    rlwinm r0, r0, 2, 0, 29
    cmplwi r3, 0x0
    lwzx r27, r4, r0
    beq @L_800845C8
    rlwinm r4, r27, 0, 23, 23
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_800845C8:
    lwz r3, 0x3c(r30)
    cmplwi r3, 0x0
    beq @L_800845E8
    rlwinm r4, r27, 0, 22, 22
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_800845E8:
    lwz r3, 0x40(r30)
    cmplwi r3, 0x0
    beq @L_80084608
    rlwinm r4, r27, 0, 21, 21
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084608:
    lwz r3, 0x44(r30)
    cmplwi r3, 0x0
    beq @L_80084628
    rlwinm r4, r27, 0, 20, 20
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084628:
    lwz r3, 0x48(r30)
    cmplwi r3, 0x0
    beq @L_80084648
    rlwinm r4, r27, 0, 19, 19
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084648:
    lbz r0, 0x20(r30)
    cmplwi r0, 0x0
    bne @L_800846FC
    rlwinm r0, r26, 2, 0, 29
    lwz r26, 0x38(r30)
    lwzx r0, r30, r0
    cmplwi r26, 0x0
    subfic r0, r0, 0x9
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r23, r0, 0, 24, 31
    beq @L_800846B4
    cmpwi r23, 0x0
    beq @L_80084698
    lha r0, 0x6(r26)
    mr r3, r29
    li r5, 0x1ba
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    b @L_800846B4
@L_80084698:
    lha r0, 0x6(r26)
    mr r3, r29
    li r5, 0x0
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    li r0, -0x1
    stw r0, 0x64(r26)
@L_800846B4:
    lwz r26, 0x40(r30)
    cmplwi r26, 0x0
    beq @L_800846FC
    cmpwi r23, 0x0
    beq @L_800846E0
    lha r0, 0x6(r26)
    mr r3, r29
    li r5, 0x1ba
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    b @L_800846FC
@L_800846E0:
    lha r0, 0x6(r26)
    mr r3, r29
    li r5, 0x0
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    li r0, -0x1
    stw r0, 0x64(r26)
@L_800846FC:
    mr r31, r30
    li r27, 0x0
    addi r26, r2, -29968
    addi r25, r25, 0x0
@L_8008470C:
    lwz r0, 0x0(r31)
    lwz r3, 0x4c(r31)
    rlwinm r0, r0, 2, 0, 29
    cmplwi r3, 0x0
    lwzx r28, r25, r0
    beq @L_80084738
    rlwinm r4, r28, 0, 31, 31
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084738:
    lwz r3, 0x5c(r31)
    cmplwi r3, 0x0
    beq @L_80084758
    rlwinm r4, r28, 0, 30, 30
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084758:
    lwz r3, 0x6c(r31)
    cmplwi r3, 0x0
    beq @L_80084778
    rlwinm r4, r28, 0, 29, 29
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084778:
    lwz r3, 0x7c(r31)
    cmplwi r3, 0x0
    beq @L_80084798
    rlwinm r4, r28, 0, 28, 28
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084798:
    lwz r3, 0x8c(r31)
    cmplwi r3, 0x0
    beq @L_800847B8
    rlwinm r4, r28, 0, 27, 27
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_800847B8:
    lwz r3, 0x9c(r31)
    cmplwi r3, 0x0
    beq @L_800847D8
    rlwinm r4, r28, 0, 26, 26
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_800847D8:
    lwz r3, 0xac(r31)
    cmplwi r3, 0x0
    beq @L_800847F8
    rlwinm r4, r28, 0, 25, 25
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_800847F8:
    lwz r3, 0xbc(r31)
    cmplwi r3, 0x0
    beq @L_80084818
    rlwinm r4, r28, 0, 24, 24
    neg r0, r4
    or r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
@L_80084818:
    lwz r0, 0x0(r31)
    cmpwi r0, 0xb
    beq @L_8008483C
    bge @L_80084864
    cmpwi r0, 0x8
    bge @L_80084864
    cmpwi r0, 0x6
    bge @L_8008483C
    b @L_80084864
@L_8008483C:
    lwz r3, 0x30(r30)
    cmplwi r3, 0x0
    beq @L_80084850
    li r4, 0x0
    bl fn_80109220
@L_80084850:
    lwz r3, 0x34(r30)
    cmplwi r3, 0x0
    beq @L_80084864
    li r4, 0x0
    bl fn_80109220
@L_80084864:
    lbz r0, 0x20(r30)
    cmplwi r0, 0x0
    bne @L_8008498C
    lwz r0, 0x0(r31)
    lwz r24, 0x4c(r31)
    subfic r0, r0, 0x2
    cntlzw r0, r0
    cmplwi r24, 0x0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r23, r0, 0, 24, 31
    beq @L_800848CC
    cmpwi r23, 0x0
    beq @L_800848B0
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x1ba
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    b @L_800848CC
@L_800848B0:
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x0
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    li r0, -0x1
    stw r0, 0x64(r24)
@L_800848CC:
    lwz r24, 0x7c(r31)
    cmplwi r24, 0x0
    beq @L_80084914
    cmpwi r23, 0x0
    beq @L_800848F8
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x1ba
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    b @L_80084914
@L_800848F8:
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x0
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    li r0, -0x1
    stw r0, 0x64(r24)
@L_80084914:
    lwz r24, 0x8c(r31)
    cmplwi r24, 0x0
    beq @L_8008495C
    cmpwi r23, 0x0
    beq @L_80084940
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x1ba
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    b @L_8008495C
@L_80084940:
    lha r0, 0x6(r24)
    mr r3, r29
    li r5, 0x0
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
    li r0, -0x1
    stw r0, 0x64(r24)
@L_8008495C:
    rlwinm r0, r28, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8008498C
    lwz r3, 0x6c(r31)
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    bne @L_8008498C
    lha r0, 0x6(r3)
    mr r3, r29
    lhz r5, 0x0(r26)
    rlwinm r4, r0, 0, 16, 31
    bl fn_801081F8
@L_8008498C:
    addi r31, r31, 0x4
    addi r26, r26, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x3
    ble @L_8008470C
@L_800849A0:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
