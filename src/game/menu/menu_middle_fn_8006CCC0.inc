    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r23, r3
    mr r24, r4
    li r31, 0x0
    li r4, 0x0
    bl fn_801040D0
    mr r0, r3
    mr r3, r23
    mr r30, r0
    li r4, 0x1
    bl fn_801040D0
    mr r21, r3
    mr r3, r23
    li r4, 0x2
    bl fn_801040D0
    mr r0, r3
    mr r3, r23
    mr r29, r0
    li r4, 0x3
    bl fn_801040D0
    lbz r0, 0x1(r23)
    mr r22, r3
    li r28, 0x0
    li r27, 0x0
    extsb r0, r0
    li r26, 0x0
    cmpwi r0, 0x4
    li r25, 0x0
    bge @L_8006CD50
    cmpwi r0, 0x0
    beq @L_8006D53C
    bge @L_8006CD58
    b @L_8006D53C
    @L_8006CD50:
    cmpwi r0, 0x6
    b @L_8006D53C
    @L_8006CD58:
    lha r0, 0x6(r24)
    cmpwi r0, 0xf2c
    beq @L_8006D108
    bge @L_8006CE2C
    cmpwi r0, 0xe79
    beq @L_8006D0B4
    bge @L_8006CDD0
    cmpwi r0, 0xe61
    beq @L_8006D06C
    bge @L_8006CDA8
    cmpwi r0, 0xe5e
    beq @L_8006D048
    bge @L_8006CD9C
    cmpwi r0, 0xe5c
    beq @L_8006D030
    bge @L_8006D03C
    b @L_8006D2C8
    @L_8006CD9C:
    cmpwi r0, 0xe60
    bge @L_8006D060
    b @L_8006D054
    @L_8006CDA8:
    cmpwi r0, 0xe76
    beq @L_8006D090
    bge @L_8006CDC4
    cmpwi r0, 0xe74
    beq @L_8006D078
    bge @L_8006D084
    b @L_8006D2C8
    @L_8006CDC4:
    cmpwi r0, 0xe78
    bge @L_8006D0A8
    b @L_8006D09C
    @L_8006CDD0:
    cmpwi r0, 0xe80
    beq @L_8006CF24
    bge @L_8006CE04
    cmpwi r0, 0xe7d
    beq @L_8006D0E4
    bge @L_8006CDF8
    cmpwi r0, 0xe7b
    beq @L_8006D0CC
    bge @L_8006D0D8
    b @L_8006D0C0
    @L_8006CDF8:
    cmpwi r0, 0xe7f
    bge @L_8006D0FC
    b @L_8006D0F0
    @L_8006CE04:
    cmpwi r0, 0xe84
    beq @L_8006D1E0
    bge @L_8006CE20
    cmpwi r0, 0xe82
    beq @L_8006D2C8
    bge @L_8006CEE0
    b @L_8006CFA4
    @L_8006CE20:
    cmpwi r0, 0xe86
    bge @L_8006D2C8
    b @L_8006D024
    @L_8006CE2C:
    cmpwi r0, 0xf4a
    beq @L_8006D198
    bge @L_8006CE90
    cmpwi r0, 0xf44
    beq @L_8006D150
    bge @L_8006CE6C
    cmpwi r0, 0xf30
    beq @L_8006D138
    bge @L_8006CE60
    cmpwi r0, 0xf2e
    beq @L_8006D120
    bge @L_8006D12C
    b @L_8006D114
    @L_8006CE60:
    cmpwi r0, 0xf32
    bge @L_8006D2C8
    b @L_8006D144
    @L_8006CE6C:
    cmpwi r0, 0xf47
    beq @L_8006D174
    bge @L_8006CE84
    cmpwi r0, 0xf46
    bge @L_8006D168
    b @L_8006D15C
    @L_8006CE84:
    cmpwi r0, 0xf49
    bge @L_8006D18C
    b @L_8006D180
    @L_8006CE90:
    cmpwi r0, 0xfc9
    beq @L_8006CEE0
    bge @L_8006CEC4
    cmpwi r0, 0xf4e
    beq @L_8006D1C8
    bge @L_8006CEB8
    cmpwi r0, 0xf4c
    beq @L_8006D1B0
    bge @L_8006D1BC
    b @L_8006D1A4
    @L_8006CEB8:
    cmpwi r0, 0xf50
    bge @L_8006D2C8
    b @L_8006D1D4
    @L_8006CEC4:
    cmpwi r0, 0x12bc
    beq @L_8006D1FC
    bge @L_8006D2C8
    cmpwi r0, 0xfcb
    beq @L_8006D024
    bge @L_8006D2C8
    b @L_8006D1E0
    @L_8006CEE0:
    cmpwi r21, 0x0
    beq @L_8006CF00
    bge @L_8006CEF4
    cmpwi r21, -0x1
    b @L_8006CF18
    @L_8006CEF4:
    cmpwi r21, 0x3
    bge @L_8006CF18
    b @L_8006CF0C
    @L_8006CF00:
    li r0, 0x3d8d
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    @L_8006CF0C:
    li r0, 0x3d8f
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    @L_8006CF18:
    li r0, 0x0
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    @L_8006CF24:
    cmplwi r22, 0x0
    beq @L_8006D2C8
    lwz r0, 0xc(r22)
    cmplwi r0, 0x6
    bgt @L_8006D2C8
    lis r3, jumptable_802EDFB0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802EDFB0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x3d91
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3d93
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3d94
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3d95
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3d9b
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3d9c
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    li r0, 0x3daf
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    @L_8006CFA4:
    cmplwi r22, 0x0
    beq @L_8006D2C8
    lwz r0, 0x4(r22)
    cmpwi r0, 0x1
    beq @L_8006CFD4
    bge @L_8006CFE0
    cmpwi r0, 0x0
    bge @L_8006CFC8
    b @L_8006CFE0
    @L_8006CFC8:
    li r0, 0x3d9d
    stw r0, 0x4c(r24)
    b @L_8006CFF4
    @L_8006CFD4:
    li r0, 0x3d9e
    stw r0, 0x4c(r24)
    b @L_8006CFF4
    @L_8006CFE0:
    lis r3, lbl_80268680@ha
    li r4, 0xb47
    addi r3, r3, lbl_80268680@l
    la r5, lbl_8047C064(r2)
    bl fn_80196E10
    @L_8006CFF4:
    mr r3, r23
    li r4, 0xe80
    bl fn_801046C8
    mr r22, r3
    lwz r3, 0x4c(r22)
    bl fn_800FA444
    lha r0, 0x50(r22)
    srwi r3, r3, 16
    add r0, r0, r3
    extsh r0, r0
    sth r0, 0x50(r24)
    b @L_8006D2C8
    @L_8006D024:
    li r0, 0x3d8b
    stw r0, 0x4c(r24)
    b @L_8006D2C8
    @L_8006D030:
    li r31, 0x1
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D03C:
    li r31, 0x1
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D048:
    li r31, 0x1
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D054:
    li r31, 0x1
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D060:
    li r31, 0x1
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D06C:
    li r31, 0x1
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D078:
    li r31, 0x2
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D084:
    li r31, 0x2
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D090:
    li r31, 0x2
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D09C:
    li r31, 0x2
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D0A8:
    li r31, 0x2
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D0B4:
    li r31, 0x2
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D0C0:
    li r31, 0x3
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D0CC:
    li r31, 0x3
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D0D8:
    li r31, 0x3
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D0E4:
    li r31, 0x3
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D0F0:
    li r31, 0x3
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D0FC:
    li r31, 0x3
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D108:
    li r31, 0x1
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D114:
    li r31, 0x1
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D120:
    li r31, 0x1
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D12C:
    li r31, 0x1
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D138:
    li r31, 0x1
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D144:
    li r31, 0x1
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D150:
    li r31, 0x2
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D15C:
    li r31, 0x2
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D168:
    li r31, 0x2
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D174:
    li r31, 0x2
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D180:
    li r31, 0x2
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D18C:
    li r31, 0x2
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D198:
    li r31, 0x3
    li r28, 0x0
    b @L_8006D2C8
    @L_8006D1A4:
    li r31, 0x3
    li r28, 0x1
    b @L_8006D2C8
    @L_8006D1B0:
    li r31, 0x3
    li r28, 0x2
    b @L_8006D2C8
    @L_8006D1BC:
    li r31, 0x3
    li r28, 0x3
    b @L_8006D2C8
    @L_8006D1C8:
    li r31, 0x3
    li r28, 0x4
    b @L_8006D2C8
    @L_8006D1D4:
    li r31, 0x3
    li r28, 0x5
    b @L_8006D2C8
    @L_8006D1E0:
    cmplwi r30, 0x0
    beq @L_8006D2C8
    mr r3, r30
    bl fn_8012AC54
    li r31, 0x4
    mr r25, r3
    b @L_8006D2C8
    @L_8006D1FC:
    cmplwi r22, 0x0
    beq @L_8006D2C8
    lwz r0, 0x0(r22)
    li r21, 0x0
    cmpwi r0, 0x1
    beq @L_8006D270
    bge @L_8006D280
    cmpwi r0, 0x0
    bge @L_8006D224
    b @L_8006D280
    @L_8006D224:
    lwz r3, 0x14(r22)
    cmpwi r3, 0x6
    beq @L_8006D260
    bge @L_8006D240
    cmpwi r3, 0x0
    bge @L_8006D24C
    b @L_8006D280
    @L_8006D240:
    cmpwi r3, 0x8
    bge @L_8006D280
    b @L_8006D268
    @L_8006D24C:
    addi r4, r3, 0x1
    li r3, 0x2f
    bl fn_80132A38
    li r21, 0x3d9f
    b @L_8006D280
    @L_8006D260:
    li r21, 0x3da0
    b @L_8006D280
    @L_8006D268:
    li r21, 0x3da1
    b @L_8006D280
    @L_8006D270:
    lwz r4, 0x14(r22)
    li r3, 0x2f
    bl fn_80132A38
    li r21, 0x3da2
    @L_8006D280:
    lwz r5, 0x64(r24)
    mr r6, r21
    li r3, 0x0
    li r4, 0x0
    bl fn_800FB680
    mr r3, r21
    bl fn_800FA444
    srwi r3, r3, 16
    lwz r4, 0x18(r22)
    addi r0, r3, 0x24
    li r3, 0x2f
    extsh r21, r0
    bl fn_80132A38
    lwz r5, 0x64(r24)
    mr r3, r21
    li r4, 0x0
    li r6, 0x3da4
    bl fn_800FB680
    @L_8006D2C8:
    cmpwi r31, 0x0
    beq @L_8006D53C
    mr r3, r30
    mr r4, r28
    bl fn_8012AC08
    mr r28, r3
    bl fn_80077A5C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006D2F8
    li r28, 0x0
    b @L_8006D328
    @L_8006D2F8:
    mr r3, r28
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006D31C
    li r28, 0x0
    li r26, 0x1
    li r27, 0x1
    b @L_8006D328
    @L_8006D31C:
    mr r3, r28
    bl fn_8011E8DC
    mr r27, r3
    @L_8006D328:
    cmpwi r31, 0x3
    beq @L_8006D460
    bge @L_8006D344
    cmpwi r31, 0x1
    beq @L_8006D350
    bge @L_8006D36C
    b @L_8006D53C
    @L_8006D344:
    cmpwi r31, 0x5
    bge @L_8006D53C
    b @L_8006D514
    @L_8006D350:
    cmplwi r28, 0x0
    beq @L_8006D53C
    mr r3, r23
    mr r4, r24
    mr r5, r28
    bl fn_8010B718
    b @L_8006D53C
    @L_8006D36C:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_8006D390
    lwz r5, 0x64(r24)
    li r3, 0x0
    li r4, 0x0
    li r6, 0x56c
    bl fn_800FB680
    b @L_8006D53C
    @L_8006D390:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_8006D3B4
    lwz r5, 0x64(r24)
    li r3, 0x0
    li r4, 0x0
    li r6, 0x56b
    bl fn_800FB680
    b @L_8006D53C
    @L_8006D3B4:
    cmplwi r28, 0x0
    beq @L_8006D53C
    mr r3, r28
    bl fn_8011F4F0
    cmplwi r3, 0x0
    beq @L_8006D3F8
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8006D3F8
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lwz r5, 0x64(r24)
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    @L_8006D3F8:
    mr r3, r28
    bl fn_8001DA60
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_8006D42C
    bge @L_8006D41C
    cmpwi r0, 0x0
    bge @L_8006D424
    b @L_8006D434
    @L_8006D41C:
    cmpwi r0, 0x3
    b @L_8006D434
    @L_8006D424:
    li r6, 0xd67
    b @L_8006D438
    @L_8006D42C:
    li r6, 0xd68
    b @L_8006D438
    @L_8006D434:
    li r6, 0x0
    @L_8006D438:
    cmplwi r6, 0x0
    beq @L_8006D53C
    lbz r0, 0x67(r24)
    cmplwi r0, 0xff
    bne @L_8006D53C
    lwz r5, 0x64(r24)
    li r3, 0x5c
    li r4, 0x0
    bl fn_800FB680
    b @L_8006D53C
    @L_8006D460:
    cmplwi r28, 0x0
    beq @L_8006D53C
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    bne @L_8006D53C
    lwz r21, 0x64(r24)
    mr r3, r28
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0x2f
    bl fn_80132A38
    cmplwi r29, 0x0
    beq @L_8006D4FC
    mr r3, r28
    mr r4, r29
    li r5, 0x0
    bl fn_800774D4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006D4F4
    mr r3, r28
    mr r4, r29
    li r5, 0x1
    bl fn_800774D4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006D4F4
    mr r3, r30
    mr r4, r29
    li r5, 0x0
    bl fn_80076F2C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006D4FC
    @L_8006D4F4:
    oris r21, r21, 0xff00
    rlwinm r21, r21, 0, 24, 7
    @L_8006D4FC:
    mr r5, r21
    li r3, 0x0
    li r4, 0x0
    li r6, 0x41fa
    bl fn_800FB680
    b @L_8006D53C
    @L_8006D514:
    cmplwi r25, 0x0
    beq @L_8006D53C
    mr r4, r25
    li r3, 0x37
    bl fn_80132A38
    lwz r5, 0x64(r24)
    li r3, 0x0
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    @L_8006D53C:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
