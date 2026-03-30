    nofralloc
    stwu r1, -0x170(r1)
    mflr r0
    stw r0, 0x174(r1)
    stmw r14, 0x128(r1)
    mr r16, r4
    mr r15, r3
    mr r17, r7
    sth r5, 0x18(r1)
    mr r18, r8
    mr r21, r9
    mr r20, r10
    stw r6, 0x1c(r1)
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
    li r5, 0x38
    li r6, 0x0
    bl fn_801FB1C0
    lis r5, lbl_8027A408@ha
    rlwinm r0, r3, 0, 24, 31
    addi r5, r5, lbl_8027A408@l
    li r4, 0x2
    stb r0, 0x10c(r1)
    addi r9, r1, 0x7c
    subi r8, r5, 0x4
    mtctr r4
@L_8023B520:
    lwz r3, 0x4(r8)
    lwzu r0, 0x8(r8)
    stw r3, 0x4(r9)
    stwu r0, 0x8(r9)
    bdnz @L_8023B520
    lwz r0, 0x4(r8)
    mr r4, r15
    addi r5, r1, 0x98
    li r3, 0x0
    stw r0, 0x4(r9)
    li r6, 0x1
    li r7, 0x1
    lhz r0, 0x8(r8)
    sth r0, 0x8(r9)
    lwz r8, -20600(r2)
    lwz r0, -20596(r2)
    stw r8, 0x28(r1)
    stw r0, 0x2c(r1)
    bl fn_801F1C18
    mr r14, r3
    mr r4, r16
    li r3, 0x0
    bl fn_801F4354
    addi r24, r1, 0x98
    mr r19, r3
    rlwinm r22, r14, 0, 16, 31
    li r27, 0x0
    b @L_8023BA38
@L_8023B590:
    clrlslwi r0, r27, 16, 2
    lwzx r14, r24, r0
    cmplwi r14, 0x0
    beq @L_8023BA34
    mr r3, r14
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023BA34
    cmplw r16, r14
    beq @L_8023BA34
    mr r3, r15
    mr r4, r14
    li r5, 0x12
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B6D0
    mr r3, r15
    mr r4, r14
    li r5, 0x3a
    bl fn_80236BFC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023B6D0
    addi r23, r1, 0x80
    li r26, 0x0
    li r25, 0x0
    b @L_8023B670
@L_8023B5FC:
    clrlslwi r0, r25, 16, 1
    lhzx r28, r23, r0
    cmplwi r28, 0xed
    bne @L_8023B62C
    mr r3, r16
    bl fn_80205B8C
    addi r4, r1, 0x22
    addi r5, r1, 0x20
    bl fn_80120B00
    lhz r0, 0x20(r1)
    cmplwi r0, 0xa
    bne @L_8023B66C
@L_8023B62C:
    mr r3, r15
    mr r4, r16
    mr r5, r28
    addi r6, r1, 0x24
    li r7, 0x1
    bl fn_80236584
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023B66C
    lha r4, 0x24(r1)
    extsh. r0, r4
    blt @L_8023B66C
    extsh r0, r26
    addi r3, r1, 0xb8
    rlwinm r0, r0, 1, 0, 30
    addi r26, r26, 0x1
    sthx r4, r3, r0
@L_8023B66C:
    addi r25, r25, 0x1
@L_8023B670:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0xb
    blt @L_8023B5FC
    extsh. r0, r26
    ble @L_8023B6D0
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    extsh r4, r26
    divw r0, r5, r4
    addi r3, r1, 0xb8
    cmplwi r21, 0x0
    mullw r0, r0, r4
    subf r0, r0, r5
    rlwinm r0, r0, 1, 0, 30
    lhax r0, r3, r0
    sth r0, 0x24(r1)
    beq @L_8023B6B8
    stw r14, 0x0(r21)
@L_8023B6B8:
    cmplwi r20, 0x0
    beq @L_8023B6C8
    lha r0, 0x24(r1)
    sth r0, 0x0(r20)
@L_8023B6C8:
    li r3, -0x1
    b @L_8023C354
@L_8023B6D0:
    mr r3, r15
    mr r4, r14
    li r5, 0x3e
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023B708
    mr r3, r15
    mr r4, r14
    li r5, 0x3f
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B7D8
@L_8023B708:
    mr r3, r15
    mr r4, r14
    bl fn_80237310
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B7D8
    li r25, 0x0
    addi r23, r1, 0x28
    mr r26, r25
    addi r28, r1, 0xb8
    b @L_8023B778
@L_8023B734:
    clrlslwi r0, r26, 16, 1
    mr r3, r15
    lhzx r5, r23, r0
    mr r4, r16
    addi r6, r1, 0x24
    li r7, 0x1
    bl fn_80236584
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023B774
    lha r3, 0x24(r1)
    extsh. r0, r3
    blt @L_8023B774
    extsh r0, r25
    addi r25, r25, 0x1
    rlwinm r0, r0, 1, 0, 30
    sthx r3, r28, r0
@L_8023B774:
    addi r26, r26, 0x1
@L_8023B778:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_8023B734
    extsh. r0, r25
    ble @L_8023B7D8
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    extsh r4, r25
    divw r0, r5, r4
    addi r3, r1, 0xb8
    cmplwi r21, 0x0
    mullw r0, r0, r4
    subf r0, r0, r5
    rlwinm r0, r0, 1, 0, 30
    lhax r0, r3, r0
    sth r0, 0x24(r1)
    beq @L_8023B7C0
    stw r14, 0x0(r21)
@L_8023B7C0:
    cmplwi r20, 0x0
    beq @L_8023B7D0
    lha r0, 0x24(r1)
    sth r0, 0x0(r20)
@L_8023B7D0:
    li r3, -0x1
    b @L_8023C354
@L_8023B7D8:
    mr r3, r15
    mr r4, r14
    li r23, 0x0
    li r5, 0x36
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B7FC
    li r23, 0x1
@L_8023B7FC:
    mr r3, r15
    mr r4, r14
    bl fn_8023785C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8023B850
    mr r3, r15
    mr r4, r14
    li r5, 0x25
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023B84C
    mr r3, r15
    mr r4, r14
    li r5, 0x4a
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B850
@L_8023B84C:
    li r23, 0x1
@L_8023B850:
    mr r3, r15
    mr r4, r14
    bl fn_8023785C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x3
    bne @L_8023B8A4
    mr r3, r15
    mr r4, r16
    li r5, 0x25
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023B8A0
    mr r3, r15
    mr r4, r16
    li r5, 0x4a
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B8A4
@L_8023B8A0:
    li r23, 0x1
@L_8023B8A4:
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B900
    mr r3, r15
    mr r4, r16
    addi r6, r1, 0x24
    li r5, 0x11d
    li r7, 0x1
    bl fn_80236584
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023B900
    lha r0, 0x24(r1)
    cmpwi r0, 0x0
    blt @L_8023B900
    cmplwi r21, 0x0
    beq @L_8023B8E8
    stw r14, 0x0(r21)
@L_8023B8E8:
    cmplwi r20, 0x0
    beq @L_8023B8F8
    lha r0, 0x24(r1)
    sth r0, 0x0(r20)
@L_8023B8F8:
    li r3, -0x1
    b @L_8023C354
@L_8023B900:
    mr r3, r15
    mr r4, r14
    li r23, 0x0
    bl fn_80235714
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023B95C
    mr r3, r15
    mr r4, r14
    li r5, 0x14
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B938
    li r23, 0x1
@L_8023B938:
    mr r3, r15
    mr r4, r14
    bl fn_8023831C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x8
    beq @L_8023B958
    cmplwi r0, 0x9
    bne @L_8023B95C
@L_8023B958:
    li r23, 0x1
@L_8023B95C:
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B9B8
    mr r3, r15
    mr r4, r16
    addi r6, r1, 0x24
    li r5, 0xcf
    li r7, 0x1
    bl fn_80236584
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023B9B8
    lha r0, 0x24(r1)
    cmpwi r0, 0x0
    blt @L_8023B9B8
    cmplwi r21, 0x0
    beq @L_8023B9A0
    stw r14, 0x0(r21)
@L_8023B9A0:
    cmplwi r20, 0x0
    beq @L_8023B9B0
    lha r0, 0x24(r1)
    sth r0, 0x0(r20)
@L_8023B9B0:
    li r3, -0x1
    b @L_8023C354
@L_8023B9B8:
    mr r3, r15
    mr r4, r14
    bl fn_80235714
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BA34
    mr r3, r15
    mr r4, r16
    bl fn_80235714
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023BA34
    mr r3, r15
    mr r4, r16
    addi r6, r1, 0x24
    li r5, 0xf4
    li r7, 0x1
    bl fn_80236584
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023BA34
    lha r0, 0x24(r1)
    cmpwi r0, 0x0
    blt @L_8023BA34
    cmplwi r21, 0x0
    beq @L_8023BA1C
    stw r14, 0x0(r21)
@L_8023BA1C:
    cmplwi r20, 0x0
    beq @L_8023BA2C
    lha r0, 0x24(r1)
    sth r0, 0x0(r20)
@L_8023BA2C:
    li r3, -0x1
    b @L_8023C354
@L_8023BA34:
    addi r27, r27, 0x1
@L_8023BA38:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r22
    blt @L_8023B590
    addi r6, r1, 0xec
    addi r5, r1, 0xcc
    addi r4, r1, 0x38
    addi r3, r1, 0x30
    li r9, 0x0
    li r7, 0x0
    b @L_8023BA7C
@L_8023BA60:
    rlwinm r8, r9, 0, 16, 31
    clrlslwi r0, r9, 16, 2
    stwx r7, r6, r0
    addi r9, r9, 0x1
    stwx r7, r5, r0
    stbx r7, r4, r8
    stbx r7, r3, r8
@L_8023BA7C:
    rlwinm r0, r9, 0, 16, 31
    cmplwi r0, 0x8
    blt @L_8023BA60
    rlwinm r0, r17, 0, 16, 31
    li r22, 0x0
    stw r0, 0x118(r1)
    li r24, 0x0
    b @L_8023BDC0
@L_8023BA9C:
    clrlslwi r26, r24, 16, 2
    rlwinm r27, r24, 0, 16, 31
    lwzx r21, r18, r26
    cmplwi r21, 0x0
    beq @L_8023BDBC
    mr r3, r21
    bl fn_80205B2C
    extsh. r0, r3
    blt @L_8023BDBC
    mr r3, r15
    mr r4, r21
    bl fn_802376EC
    rlwinm r0, r3, 0, 16, 31
    li r23, 0x0
    stw r0, 0x110(r1)
    li r25, 0x0
    lhz r0, 0x18(r1)
    stw r0, 0x114(r1)
    b @L_8023BD94
@L_8023BAE8:
    lwz r3, 0x1c(r1)
    clrlslwi r0, r25, 16, 1
    lhzx r20, r3, r0
    cmplwi r20, 0x0
    beq @L_8023BD90
    cmplwi r20, 0x165
    beq @L_8023BD90
    mr r3, r15
    mr r4, r16
    mr r5, r20
    mr r6, r21
    bl fn_8023C530
    mr r31, r3
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BB34
    li r0, 0x1
    addi r3, r1, 0x30
    stbx r0, r3, r27
@L_8023BB34:
    mr r3, r15
    mr r4, r20
    li r5, 0x1
    bl fn_8023943C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023BD90
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
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BBBC
    mr r4, r20
    li r3, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_8011BEB4
    mr. r28, r3
    bne @L_8023BBC4
    lis r3, -32731
    addi r28, r3, 14664
    b @L_8023BBC4
@L_8023BBBC:
    li r30, 0x0
    b @L_8023BCA0
@L_8023BBC4:
    mr r4, r15
    addi r5, r1, 0x60
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r14, r3
    mr r4, r20
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    li r30, 0x0
    cmplwi r0, 0x7
    bgt @L_8023BCA0
    lis r3, jumptable_8039A578@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A578@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r12, r28
    mr r3, r15
    mr r4, r16
    mr r5, r20
    mr r6, r21
    mtctr r12
    bctrl
    mr r30, r3
    b @L_8023BCA0
    rlwinm r14, r14, 0, 16, 31
    li r29, 0x0
    b @L_8023BC70
@L_8023BC4C:
    mr r12, r28
    mr r3, r15
    mr r4, r16
    mr r5, r20
    mr r6, r21
    mtctr r12
    bctrl
    add r30, r30, r3
    addi r29, r29, 0x1
@L_8023BC70:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r14
    blt @L_8023BC4C
    b @L_8023BCA0
    mr r12, r28
    mr r3, r15
    mr r4, r16
    mr r5, r20
    mr r6, r21
    mtctr r12
    bctrl
    mr r30, r3
@L_8023BCA0:
    cmpw r23, r30
    bge @L_8023BCAC
    mr r23, r30
@L_8023BCAC:
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
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BD1C
    mr r4, r20
    li r3, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_8011BEB4
    mr. r14, r3
    bne @L_8023BD24
    lis r3, -32731
    addi r14, r3, 14664
    b @L_8023BD24
@L_8023BD1C:
    li r3, 0x0
    b @L_8023BD6C
@L_8023BD24:
    mr r4, r15
    addi r5, r1, 0x40
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r4, r20
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    mr r12, r14
    mr r3, r15
    mr r4, r16
    mr r5, r20
    mr r6, r21
    mtctr r12
    bctrl
@L_8023BD6C:
    lwz r0, 0x110(r1)
    cmpw r0, r3
    bge @L_8023BD90
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BD90
    li r0, 0x1
    addi r3, r1, 0x38
    stbx r0, r3, r27
@L_8023BD90:
    addi r25, r25, 0x1
@L_8023BD94:
    lwz r0, 0x114(r1)
    rlwinm r3, r25, 0, 16, 31
    cmplw r3, r0
    blt @L_8023BAE8
    addi r3, r1, 0xcc
    stwx r23, r3, r26
    lwzx r0, r3, r26
    cmpw r22, r0
    bge @L_8023BDBC
    mr r22, r0
@L_8023BDBC:
    addi r24, r24, 0x1
@L_8023BDC0:
    lwz r0, 0x118(r1)
    rlwinm r3, r24, 0, 16, 31
    cmplw r3, r0
    blt @L_8023BA9C
    lbz r0, 0x10c(r1)
    rlwinm r23, r17, 0, 16, 31
    li r14, 0x0
    clrlslwi r3, r0, 24, 1
    addi r26, r3, 0x1
    b @L_8023C2B8
@L_8023BDE8:
    clrlslwi r27, r14, 16, 2
    rlwinm r29, r14, 0, 16, 31
    lwzx r20, r18, r27
    cmplwi r20, 0x0
    beq @L_8023C2B4
    mr r3, r20
    bl fn_80205B2C
    mr r25, r3
    extsh. r0, r25
    blt @L_8023C2B4
    mr r4, r20
    li r3, 0x0
    bl fn_801F4354
    mr r0, r3
    mr r3, r20
    mr r21, r0
    bl fn_80205B8C
    mr r24, r3
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x227
    mr r7, r3
    addi r28, r1, 0xec
    stw r0, 0xc(r1)
    mr r6, r19
    mr r8, r21
    mr r9, r24
    lwzx r0, r28, r27
    subi r3, r5, 0x1394
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    stw r0, 0x10(r1)
    li r10, 0x0
    bl fn_8023A118
    rlwinm r24, r25, 0, 16, 31
    mr r3, r15
    mr r4, r24
    li r5, 0x52
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BEF0
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x34
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r25, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r25
    li r8, 0x0
    li r9, 0x0
    li r10, 0x34
    bl fn_80239EE8
@L_8023BEF0:
    mr r3, r15
    mr r4, r24
    li r5, 0x53
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BF60
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x35
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r25, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r25
    li r8, 0x0
    li r9, 0x0
    li r10, 0x35
    bl fn_80239EE8
@L_8023BF60:
    mr r3, r15
    mr r4, r24
    li r5, 0x55
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023BFD0
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x36
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r25, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r25
    li r8, 0x0
    li r9, 0x0
    li r10, 0x36
    bl fn_80239EE8
@L_8023BFD0:
    addi r3, r1, 0xcc
    lwzx r0, r3, r27
    cmpw r22, r0
    bgt @L_8023C030
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x37
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r25, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r25
    li r8, 0x0
    li r9, 0x0
    li r10, 0x37
    bl fn_80239EE8
@L_8023C030:
    mr r4, r15
    mr r8, r20
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x0
    bl fn_801F1990
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C0A8
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x38
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r25, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r25
    li r8, 0x0
    li r9, 0x0
    li r10, 0x38
    bl fn_80239EE8
@L_8023C0A8:
    mr r3, r15
    mr r4, r24
    li r5, 0x56
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x1
    bne @L_8023C114
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x39
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r24, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r24
    li r8, 0x0
    li r9, 0x0
    li r10, 0x39
    bl fn_80239EE8
@L_8023C114:
    addi r3, r1, 0x30
    lbzx r0, r3, r29
    cmplwi r0, 0x0
    bne @L_8023C174
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x3a
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r24, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r24
    li r8, 0x0
    li r9, 0x0
    li r10, 0x3a
    bl fn_80239EE8
@L_8023C174:
    addi r3, r1, 0x38
    lbzx r0, r3, r29
    cmplwi r0, 0x1
    bne @L_8023C1D4
    lwzx r3, r28, r27
    mr r4, r15
    li r5, 0x3b
    bl fn_80239984
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r24, r3
    mr r3, r16
    bl fn_80205B8C
    lis r8, 0x1
    mr r5, r3
    subi r3, r8, 0x1394
    mr r4, r19
    mr r6, r21
    mr r7, r24
    li r8, 0x0
    li r9, 0x0
    li r10, 0x3b
    bl fn_80239EE8
@L_8023C1D4:
    bl fn_8000815C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C260
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    lwzx r3, r28, r27
    divw r0, r4, r26
    mullw r0, r0, r26
    subf r4, r0, r4
    lbz r0, 0x10c(r1)
    subf r25, r0, r4
    mr r4, r25
    bl fn_802399FC
    stwx r3, r28, r27
    mr r3, r20
    bl fn_80205B8C
    mr r24, r3
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x225
    mr r7, r3
    mr r6, r19
    stw r0, 0xc(r1)
    mr r8, r21
    mr r9, r24
    subi r3, r5, 0x1394
    stw r25, 0x10(r1)
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    li r10, 0x0
    bl fn_8023A118
@L_8023C260:
    mr r3, r20
    bl fn_80205B8C
    mr r20, r3
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x226
    mr r7, r3
    mr r6, r19
    stw r0, 0xc(r1)
    mr r8, r21
    mr r9, r20
    subi r3, r5, 0x1394
    lwzx r0, r28, r27
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    li r10, 0x0
    stw r0, 0x10(r1)
    bl fn_8023A118
@L_8023C2B4:
    addi r14, r14, 0x1
@L_8023C2B8:
    rlwinm r0, r14, 0, 16, 31
    cmplw r0, r23
    blt @L_8023BDE8
    mr r4, r17
    addi r3, r1, 0xec
    li r5, 0x1
    bl fn_802397B8
    mr. r20, r3
    bge @L_8023C2E4
    li r3, -0x1
    b @L_8023C354
@L_8023C2E4:
    rlwinm r15, r20, 2, 0, 29
    li r3, 0x0
    lwzx r4, r18, r15
    bl fn_801F4354
    mr r17, r3
    lwzx r3, r18, r15
    bl fn_80205B8C
    mr r14, r3
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    addi r4, r1, 0xec
    stw r0, 0x8(r1)
    li r0, 0x228
    lis r5, 0x1
    mr r7, r3
    stw r0, 0xc(r1)
    mr r6, r19
    mr r8, r17
    mr r9, r14
    lwzx r0, r4, r15
    subi r3, r5, 0x1394
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    stw r0, 0x10(r1)
    li r10, 0x0
    bl fn_8023A118
    mr r3, r20
@L_8023C354:
    lmw r14, 0x128(r1)
    lwz r0, 0x174(r1)
    mtlr r0
    addi r1, r1, 0x170
    blr
