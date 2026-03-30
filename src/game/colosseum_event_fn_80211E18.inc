    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x54(r1)
    stmw r17, 0x14(r1)
    mr r21, r3
    mr r31, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r20, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r19, r0
    li r4, 0x0
    bl fn_801F025C
    mr r22, r3
    mr r4, r19
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    mr r29, r3
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    mr r3, r19
    mr r23, r0
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    mr. r17, r3
    beq @L_802126B0
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_80142CF4
    mr r3, r17
    li r4, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_80142CF4
    mr r24, r3
    mr r3, r17
    li r4, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r28, r3, 0, 24, 31
    mr r3, r31
    bl fn_801440A0
    bl fn_80143DFC
    bl fn_80143A94
    lis r4, -32712
    mr r0, r3
    addi r4, r4, -24744
    mr r3, r19
    addis r30, r4, 0x1
    mr r17, r0
    lbz r26, 0x601e(r30)
    li r4, 0x2e
    lbz r27, 0x60a4(r30)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80211F40
    mr r3, r19
    li r4, 0x2e
    bl fn_80202810
@L_80211F40:
    mr r3, r19
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80211F64
    mr r3, r19
    li r4, 0x15
    bl fn_80202810
@L_80211F64:
    mr r3, r19
    li r4, 0x28
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80211F88
    mr r3, r19
    li r4, 0x28
    bl fn_80202810
@L_80211F88:
    cmplwi r28, 0x1
    bne @L_80211F9C
    lis r3, -32713
    lwz r17, 24100(r3)
    b @L_802125DC
@L_80211F9C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80211FD0
    lis r3, -32713
    clrlslwi r0, r31, 16, 2
    addi r3, r3, 24048
    lwzx r17, r3, r0
    b @L_802125DC
@L_80211FD0:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x50
    beq @L_80211FE4
    cmplwi r0, 0x51
    bne @L_80211FF0
@L_80211FE4:
    lis r3, -32713
    lwz r17, 24132(r3)
    b @L_802125DC
@L_80211FF0:
    mr r3, r31
    bl fn_801440A0
    bl fn_80143DFC
    bl fn_80143A94
    mr. r25, r3
    bne @L_80212010
    li r25, 0x7
    b @L_8021214C
@L_80212010:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x13
    bne @L_80212024
    li r25, 0x1
    b @L_8021214C
@L_80212024:
    bl fn_801437E0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80212038
    li r25, 0x2
    b @L_8021214C
@L_80212038:
    mr r3, r25
    bl fn_80143940
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802120B0
    mr r3, r25
    bl fn_80143918
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802120B0
    mr r3, r25
    bl fn_801438F0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802120B0
    mr r3, r25
    bl fn_801438C8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802120B0
    mr r3, r25
    bl fn_801438A0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802120B0
    mr r3, r25
    bl fn_80143878
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802120B8
@L_802120B0:
    li r25, 0x3
    b @L_8021214C
@L_802120B8:
    mr r3, r25
    bl fn_80143A44
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802120D4
    li r25, 0x4
    b @L_8021214C
@L_802120D4:
    mr r3, r25
    bl fn_80143A28
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212124
    mr r3, r25
    bl fn_80143A0C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212124
    mr r3, r25
    bl fn_801439F0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212124
    mr r3, r25
    bl fn_801439D4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212124
    mr r3, r25
    bl fn_801439B8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021212C
@L_80212124:
    li r25, 0x5
    b @L_8021214C
@L_8021212C:
    mr r3, r25
    bl fn_80143990
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212148
    li r25, 0x6
    b @L_8021214C
@L_80212148:
    li r25, 0x7
@L_8021214C:
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x7
    bne @L_80212164
    lis r3, -32713
    lwz r17, 24100(r3)
    b @L_802125DC
@L_80212164:
    mr r7, r19
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r25, 0, 24, 31
    li r3, 0x0
    addi r18, r13, -31400
    cmpwi r0, 0x4
    stb r3, 0x5(r18)
    beq @L_802123B0
    bge @L_802121A4
    cmpwi r0, 0x3
    bge @L_802121B4
    b @L_802125CC
@L_802121A4:
    cmpwi r0, 0x6
    beq @L_802125CC
    bge @L_802125CC
    b @L_802123BC
@L_802121B4:
    mr r3, r17
    bl fn_80143940
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122F0
    mr r3, r17
    bl fn_80143918
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122F0
    mr r3, r17
    bl fn_801438F0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122F0
    mr r3, r17
    bl fn_801438C8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122F0
    mr r3, r17
    bl fn_801438A0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122F0
    mr r3, r19
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021223C
    li r0, 0x5
    stb r0, 0x5(r18)
    b @L_802125CC
@L_8021223C:
    mr r3, r19
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212260
    li r0, 0x4
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212260:
    mr r3, r19
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212284
    li r0, 0x4
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212284:
    mr r3, r19
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122A8
    li r0, 0x3
    stb r0, 0x5(r18)
    b @L_802125CC
@L_802122A8:
    mr r3, r19
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802122CC
    li r0, 0x2
    stb r0, 0x5(r18)
    b @L_802125CC
@L_802122CC:
    mr r3, r19
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802125CC
    li r0, 0x1
    stb r0, 0x5(r18)
    b @L_802125CC
@L_802122F0:
    mr r3, r17
    bl fn_80143940
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212310
    li r0, 0x5
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212310:
    mr r3, r17
    bl fn_80143918
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212330
    li r0, 0x4
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212330:
    mr r3, r17
    bl fn_801438F0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212350
    li r0, 0x3
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212350:
    mr r3, r17
    bl fn_801438C8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212370
    li r0, 0x2
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212370:
    mr r3, r17
    bl fn_801438A0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212390
    li r0, 0x1
    stb r0, 0x5(r18)
    b @L_802125CC
@L_80212390:
    mr r3, r17
    bl fn_80143878
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802125CC
    li r0, 0x0
    stb r0, 0x5(r18)
    b @L_802125CC
@L_802123B0:
    li r0, 0x5
    stb r0, 0x5(r18)
    b @L_802125CC
@L_802123BC:
    li r0, 0x4
    mr r3, r17
    stb r0, 0x5(r18)
    bl fn_80143A28
    rlwinm. r19, r3, 0, 24, 31
    ble @L_802123FC
    lis r3, -32728
    addi r3, r3, -24964
    lwz r3, 0x4(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x1
    stb r0, 0x601e(r30)
    b @L_802124D8
@L_802123FC:
    mr r3, r17
    bl fn_80143A0C
    rlwinm. r19, r3, 0, 24, 31
    ble @L_80212434
    lis r3, -32728
    addi r3, r3, -24964
    lwz r3, 0x8(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x2
    stb r0, 0x601e(r30)
    b @L_802124D8
@L_80212434:
    mr r3, r17
    bl fn_801439F0
    rlwinm. r19, r3, 0, 24, 31
    ble @L_8021246C
    lis r3, -32728
    addi r3, r3, -24964
    lwz r3, 0xc(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x3
    stb r0, 0x601e(r30)
    b @L_802124D8
@L_8021246C:
    mr r3, r17
    bl fn_801439D4
    rlwinm. r19, r3, 0, 24, 31
    ble @L_802124A4
    lis r3, -32728
    addi r3, r3, -24964
    lwz r3, 0x18(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x6
    stb r0, 0x601e(r30)
    b @L_802124D8
@L_802124A4:
    mr r3, r17
    bl fn_801439B8
    rlwinm. r19, r3, 0, 24, 31
    ble @L_802124D8
    lis r3, -32728
    addi r3, r3, -24964
    lwz r3, 0x10(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x4
    stb r0, 0x601e(r30)
@L_802124D8:
    extsh. r0, r19
    bge @L_80212558
    extsh r0, r19
    cmpwi r0, 0x1
    beq @L_802124F4
    cmpwi r0, -0x1
    bne @L_8021251C
@L_802124F4:
    li r3, 0x76bd
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    lbz r0, 0x601e(r30)
    rlwinm r3, r0, 0, 28, 31
    addi r0, r3, 0x15
    stb r0, 0x601e(r30)
    b @L_80212540
@L_8021251C:
    li r3, 0x7628
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    lbz r0, 0x601e(r30)
    rlwinm r3, r0, 0, 28, 31
    addi r0, r3, 0x2d
    stb r0, 0x60a4(r30)
@L_80212540:
    li r3, 0x7629
    bl fn_800FA280
    mr r4, r3
    li r3, 0x41
    bl fn_80132A38
    b @L_802125CC
@L_80212558:
    extsh r0, r19
    cmpwi r0, 0x1
    beq @L_8021256C
    cmpwi r0, -0x1
    bne @L_80212594
@L_8021256C:
    li r3, 0x76bd
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    lbz r0, 0x601e(r30)
    rlwinm r3, r0, 0, 28, 31
    addi r0, r3, 0xe
    stb r0, 0x60a4(r30)
    b @L_802125B8
@L_80212594:
    li r3, 0x7626
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    lbz r0, 0x601e(r30)
    rlwinm r3, r0, 0, 28, 31
    addi r0, r3, 0x26
    stb r0, 0x60a4(r30)
@L_802125B8:
    li r3, 0x7627
    bl fn_800FA280
    mr r4, r3
    li r3, 0x41
    bl fn_80132A38
@L_802125CC:
    lis r3, -32713
    clrlslwi r0, r25, 24, 2
    addi r3, r3, 24100
    lwzx r17, r3, r0
@L_802125DC:
    li r0, 0x0
    mr r3, r29
    stb r0, -21004(r13)
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r29
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    mr r3, r29
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    rlwinm r4, r31, 0, 16, 31
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x29
    bl fn_80132A38
    li r3, 0x1
    bl fn_801EF8F4
    cmplwi r28, 0x0
    bne @L_80212664
    mr r3, r22
    mr r4, r20
    li r5, 0x1
    bl fn_80265598
@L_80212664:
    mr r3, r21
    mr r4, r17
    li r5, 0x1
    bl fn_80211B94
    cmplwi r28, 0x0
    bne @L_80212690
    mr r3, r23
    mr r4, r31
    extsh r6, r24
    li r5, 0x1
    bl fn_801299C8
@L_80212690:
    stb r26, 0x601e(r30)
    stb r27, 0x60a4(r30)
    bl fn_801DA7AC
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
@L_802126B0:
    lmw r17, 0x14(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
