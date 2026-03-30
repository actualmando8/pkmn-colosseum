    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x43
    stw r0, 0x124(r1)
    li r6, 0x0
    stmw r14, 0xd8(r1)
    mr r15, r3
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r15
    li r4, 0x1
    bl fn_801F849C
    addi r4, r1, 0x88
    addi r3, r1, 0x34
    li r6, 0x0
    li r5, 0x0
    b @L_80234A74
@L_80234A64:
    clrlslwi r0, r6, 16, 2
    addi r6, r6, 0x1
    stwx r5, r4, r0
    stwx r5, r3, r0
@L_80234A74:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0x8
    blt @L_80234A64
    mr r4, r15
    addi r5, r1, 0xa8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    stw r3, 0xcc(r1)
    mr r4, r15
    addi r5, r1, 0x68
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    lwz r0, 0xcc(r1)
    lis r5, 0x1
    lis r4, 0xffff
    stw r3, 0xd0(r1)
    subi r19, r5, 0x1
    rlwinm r0, r0, 0, 16, 31
    addi r24, r4, 0x1
    sth r19, 0xc8(r1)
    mr r16, r24
    li r17, 0xff
    stw r0, 0xd4(r1)
    li r25, 0x0
    b @L_8023540C
@L_80234AE8:
    clrlslwi r27, r25, 16, 2
    addi r3, r1, 0xa8
    lwzx r23, r3, r27
    cmplwi r23, 0x0
    beq @L_80235408
    mr r3, r23
    bl fn_80205B2C
    extsh. r0, r3
    blt @L_80235408
    mr r3, r23
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r14, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r14
    bl fn_80203DAC
    mr r14, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r30, 0x0
    addi r3, r1, 0x54
    mr r4, r30
    b @L_80234BB8
@L_80234BA4:
    addic. r0, r1, 0x54
    beq @L_80234BB4
    clrlslwi r0, r4, 16, 1
    sthx r30, r3, r0
@L_80234BB4:
    addi r4, r4, 0x1
@L_80234BB8:
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0xa
    blt @L_80234BA4
    li r20, 0x0
    b @L_80234C74
@L_80234BCC:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    mr r3, r18
    bl fn_80205BE8
    rlwinm r6, r20, 0, 24, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r4, r3, 0, 16, 31
    beq @L_80234C70
    cmplwi r4, 0x165
    bne @L_80234C58
    cmplwi r4, 0x163
    beq @L_80234C70
@L_80234C58:
    addic. r0, r1, 0x54
    beq @L_80234C6C
    clrlslwi r0, r30, 16, 1
    addi r3, r1, 0x54
    sthx r4, r3, r0
@L_80234C6C:
    addi r30, r30, 0x1
@L_80234C70:
    addi r20, r20, 0x1
@L_80234C74:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80234BCC
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r23
    bl fn_80203A6C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r19, 0, 16, 31
    cmplw r0, r3
    ble @L_80234CC4
    mr r19, r3
@L_80234CC4:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80203E7C
    rlwinm r4, r17, 0, 24, 31
    rlwinm r0, r3, 0, 24, 31
    cmplw r4, r0
    ble @L_80234D20
    mr r17, r3
@L_80234D20:
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r14
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8012640C
    rlwinm r18, r3, 0, 16, 31
    mr r4, r14
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    lhz r3, 0xc8(r1)
    add r18, r18, r4
    rlwinm r0, r18, 0, 16, 31
    cmplw r3, r0
    ble @L_80234D90
    sth r18, 0xc8(r1)
@L_80234D90:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r14, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r14
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8012640C
    addi r14, r1, 0x34
    rlwinm r6, r3, 0, 16, 31
    lwzx r0, r14, r27
    mr r3, r23
    li r4, 0x0
    li r5, 0xd6
    add r0, r0, r6
    li r6, 0x0
    stwx r0, r14, r27
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_8012640C
    lwzx r0, r14, r27
    rlwinm r5, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    add r0, r0, r5
    li r5, 0xd6
    stwx r0, r14, r27
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8012640C
    lwzx r0, r14, r27
    rlwinm r5, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    add r0, r0, r5
    li r5, 0xd6
    stwx r0, r14, r27
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8012640C
    lwzx r0, r14, r27
    rlwinm r5, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    add r0, r0, r5
    li r5, 0xd6
    stwx r0, r14, r27
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8012640C
    lwzx r0, r14, r27
    rlwinm r5, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    add r0, r0, r5
    li r5, 0xd6
    stwx r0, r14, r27
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_8012640C
    lwzx r0, r14, r27
    rlwinm r5, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    add r0, r0, r5
    li r5, 0xd6
    stwx r0, r14, r27
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    bl fn_80203E7C
    lwzx r4, r14, r27
    rlwinm r0, r3, 0, 24, 31
    mullw r0, r4, r0
    stwx r0, r14, r27
    lwzx r0, r14, r27
    cmpw r16, r0
    bge @L_8023515C
    mr r16, r0
@L_8023515C:
    lwz r0, 0xd0(r1)
    addi r26, r1, 0x88
    li r21, 0x0
    rlwinm r14, r0, 0, 16, 31
    b @L_802353EC
@L_80235170:
    clrlslwi r0, r21, 16, 2
    addi r3, r1, 0x68
    lwzx r22, r3, r0
    cmplwi r22, 0x0
    beq @L_802353E8
    rlwinm r31, r30, 0, 16, 31
    li r20, 0x0
    b @L_802353DC
@L_80235190:
    clrlslwi r0, r20, 16, 1
    addi r3, r1, 0x54
    lhzx r18, r3, r0
    cmplwi r18, 0x0
    beq @L_802353D8
    cmplwi r18, 0x165
    beq @L_802353D8
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    li r0, 0x0
    li r5, 0xd
    lis r4, -32733
    stw r5, 0x20(r1)
    mr r28, r3
    addi r5, r1, 0x20
    stw r0, 0x24(r1)
    addi r4, r4, 25192
    li r3, 0x0
    li r6, 0x0
    stw r0, 0x28(r1)
    stw r0, 0x2c(r1)
    stw r15, 0x30(r1)
    bl fn_801F37B0
    lwz r0, 0x24(r1)
    cmplwi r0, 0x0
    beq @L_80235230
    li r28, 0x0
    b @L_80235274
@L_80235230:
    li r0, 0x0
    li r4, 0x4d
    lis r3, -32733
    stw r4, 0xc(r1)
    addi r4, r3, 25192
    addi r5, r1, 0xc
    stw r0, 0x10(r1)
    li r3, 0x0
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r0, 0x18(r1)
    stw r15, 0x1c(r1)
    bl fn_801F37B0
    lwz r0, 0x10(r1)
    cmplwi r0, 0x0
    beq @L_80235274
    li r28, 0x0
@L_80235274:
    mr r4, r29
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235368
    cmplwi r18, 0xa5
    beq @L_802352B4
    cmplwi r18, 0x164
    beq @L_802352B4
    cmplwi r18, 0xf8
    beq @L_802352B4
    cmplwi r18, 0x161
    bne @L_802352BC
@L_802352B4:
    li r0, 0x9
    b @L_8023536C
@L_802352BC:
    cmplwi r18, 0xed
    bne @L_802352E0
    mr r3, r23
    bl fn_80205B8C
    addi r5, r1, 0x8
    li r4, 0x0
    bl fn_80120B00
    lhz r0, 0x8(r1)
    b @L_8023536C
@L_802352E0:
    cmplwi r18, 0x137
    bne @L_8023534C
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80235300
    li r0, 0xb
    sth r0, 0x8(r1)
    b @L_80235344
@L_80235300:
    cmplwi r0, 0x3
    bne @L_80235314
    li r0, 0x5
    sth r0, 0x8(r1)
    b @L_80235344
@L_80235314:
    cmplwi r0, 0x1
    bne @L_80235328
    li r0, 0xa
    sth r0, 0x8(r1)
    b @L_80235344
@L_80235328:
    cmplwi r0, 0x4
    bne @L_8023533C
    li r0, 0xf
    sth r0, 0x8(r1)
    b @L_80235344
@L_8023533C:
    li r0, 0x0
    sth r0, 0x8(r1)
@L_80235344:
    lhz r0, 0x8(r1)
    b @L_8023536C
@L_8023534C:
    mr r4, r18
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    b @L_8023536C
@L_80235368:
    li r0, 0x9
@L_8023536C:
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_802353D8
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r18
    li r4, 0x1
    bl fn_8011BAC0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802353D8
    mr r3, r15
    mr r4, r23
    mr r5, r18
    mr r6, r22
    li r7, 0x0
    bl fn_8023C370
    lwzx r0, r26, r27
    add r0, r0, r3
    stwx r0, r26, r27
@L_802353D8:
    addi r20, r20, 0x1
@L_802353DC:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r31
    blt @L_80235190
@L_802353E8:
    addi r21, r21, 0x1
@L_802353EC:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r14
    blt @L_80235170
    lwzx r0, r26, r27
    cmpw r24, r0
    bge @L_80235408
    mr r24, r0
@L_80235408:
    addi r25, r25, 0x1
@L_8023540C:
    lwz r0, 0xd4(r1)
    rlwinm r3, r25, 0, 16, 31
    cmplw r3, r0
    blt @L_80234AE8
    lwz r0, 0xcc(r1)
    rlwinm r19, r19, 0, 16, 31
    addi r18, r1, 0xa8
    li r21, 0x0
    rlwinm r22, r0, 0, 16, 31
    b @L_8023563C
@L_80235434:
    clrlslwi r20, r21, 16, 2
    lwzx r26, r18, r20
    cmplwi r26, 0x0
    beq @L_80235638
    mr r3, r26
    bl fn_80205B2C
    mr r14, r3
    extsh. r0, r14
    blt @L_80235638
    mr r3, r26
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r23
    bl fn_80203DAC
    mr r25, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r26
    bl fn_80203A6C
    cmpw r19, r3
    blt @L_802354F4
    mr r3, r15
    rlwinm r4, r14, 0, 16, 31
    li r5, 0x52
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
@L_802354F4:
    mr r3, r26
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r23
    bl fn_80203E7C
    rlwinm r4, r17, 0, 24, 31
    rlwinm r0, r3, 0, 24, 31
    cmplw r4, r0
    blt @L_80235564
    mr r3, r15
    rlwinm r4, r14, 0, 16, 31
    li r5, 0x53
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
@L_80235564:
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r25
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8012640C
    rlwinm r23, r3, 0, 16, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    lhz r3, 0xc8(r1)
    add r23, r23, r4
    rlwinm r0, r23, 0, 16, 31
    cmplw r3, r0
    blt @L_802355E8
    mr r3, r15
    rlwinm r4, r14, 0, 16, 31
    li r5, 0x54
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
@L_802355E8:
    addi r3, r1, 0x88
    lwzx r0, r3, r20
    cmpw r24, r0
    bgt @L_80235610
    mr r3, r15
    rlwinm r4, r14, 0, 16, 31
    li r5, 0x55
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
@L_80235610:
    addi r3, r1, 0x34
    lwzx r0, r3, r20
    cmpw r16, r0
    bgt @L_80235638
    mr r3, r15
    rlwinm r4, r14, 0, 16, 31
    li r5, 0x56
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
@L_80235638:
    addi r21, r21, 0x1
@L_8023563C:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r22
    blt @L_80235434
    lmw r14, 0xd8(r1)
    lwz r0, 0x124(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
