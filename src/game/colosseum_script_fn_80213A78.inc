    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    li r31, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r26, r3
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0xa
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r22, r3, 0, 16, 31
    mr r4, r26
    li r3, 0x0
    bl fn_801F4354
    mr r29, r3
    mr r4, r26
    li r3, 0x0
    bl fn_801F4220
    mr r0, r3
    li r3, 0x12
    mr r24, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    mr r4, r30
    bl fn_801F4354
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    bl fn_801F54A4
    mr r0, r3
    mr r3, r30
    mr r25, r0
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    bl fn_80205BE8
    mr r0, r3
    li r3, 0x0
    mr r27, r0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r24
    mr r4, r27
    mr r5, r25
    mr r6, r22
    li r7, 0x1
    bl fn_80129F20
    mr r26, r3
    extsh r0, r26
    cmpwi r0, -0x1
    bne @L_80213BB8
    li r31, 0x1
@L_80213BB8:
    rlwinm. r0, r31, 0, 24, 31
    bne @L_80213CBC
    extsh. r0, r26
    blt @L_80213CBC
    mr r3, r29
    mr r4, r23
    bl fn_801F7F80
    mr. r25, r3
    beq @L_80213CBC
    mr r3, r24
    rlwinm r5, r26, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr. r24, r3
    beq @L_80213CBC
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213CBC
    mr r3, r24
    li r4, 0x4
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213C2C
    mr r3, r24
    li r4, 0x4
    li r5, 0x1
    bl fn_8012190C
@L_80213C2C:
    li r3, 0x0
    bl fn_801F4804
    mr r5, r3
    mr r3, r25
    mr r4, r24
    bl fn_80206AEC
    mr r3, r25
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213CBC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213CBC
    mr r3, r29
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213CBC
    mr r3, r25
    li r4, 0x3
    bl fn_802032E4
@L_80213CBC:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213D08
    mr r3, r29
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213D08
    mr r3, r27
    li r4, 0x1
    bl fn_80123368
    mr r3, r27
    li r4, 0x1
    bl fn_801232E0
@L_80213D08:
    mr r3, r27
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lis r3, -32733
    mr r5, r30
    addi r4, r3, 12260
    li r6, 0x0
    li r3, 0x0
    bl fn_801F37B0
    mr r3, r30
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r30
    li r4, 0x0
    bl fn_80202998
    mr r3, r30
    li r4, 0x17
    bl fn_80202810
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213DA0
    mr r3, r30
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80213DA0:
    mr r3, r30
    li r4, 0x3e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213DEC
    mr r3, r30
    li r4, 0x3e
    bl fn_80202810
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213DEC
    mr r3, r30
    li r4, 0xc8
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80213DEC:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213E38
    mr r3, r30
    li r4, 0xd2
    li r5, 0x0
    bl fn_801FE710
@L_80213E38:
    mr r3, r28
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213E74
    li r3, 0x1
    bl fn_801EF8F4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80213E80
@L_80213E74:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80213E80:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
