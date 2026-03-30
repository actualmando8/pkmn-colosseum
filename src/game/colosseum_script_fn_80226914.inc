    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x44(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r19, 0xc(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r28, r3
    bl fn_80205184
    mr r30, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r25, r3
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mr r0, r3
    mr r3, r25
    mr r24, r0
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_8011BEB4
    mr r29, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r25
    mr r27, r0
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226EA4
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r22, r3
    bl fn_80205B8C
    li r4, 0x0
    mr r21, r3
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r21
    mr r23, r0
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r22
    mr r20, r0
    bl fn_801FEF74
    mr. r19, r3
    beq @L_80226AD4
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 23, 23
    bne @L_80226AD4
    cmpw r19, r24
    blt @L_80226A5C
    mr r21, r24
    subf r19, r24, r19
    b @L_80226A64
@L_80226A5C:
    mr r21, r19
    li r19, 0x0
@L_80226A64:
    mr r3, r22
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80226A98
    mr r3, r22
    mr r7, r21
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_801254B4
@L_80226A98:
    mr r3, r25
    mr r7, r21
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r22
    mr r4, r19
    bl fn_801FED3C
    li r3, 0x75b1
    bl fn_802624CC
    li r3, 0x40
    bl fn_801F000C
    bl fn_8026246C
    b @L_80226EEC
@L_80226AD4:
    lwz r0, lbl_8047B618(r13)
    cmpwi r24, 0x0
    rlwinm r0, r0, 0, 24, 22
    stw r0, lbl_8047B618(r13)
    bge @L_80226AFC
    subf r23, r24, r23
    cmpw r23, r20
    ble @L_80226DFC
    mr r23, r20
    b @L_80226DFC
@L_80226AFC:
    cmpw r23, r24
    ble @L_80226B10
    mr r29, r24
    subf r23, r24, r23
    b @L_80226B18
@L_80226B10:
    mr r29, r23
    li r23, 0x0
@L_80226B18:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 26, 26
    beq @L_80226B30
    rlwinm r0, r3, 0, 27, 25
    stw r0, lbl_8047B618(r13)
    b @L_80226BD8
@L_80226B30:
    mr r3, r22
    li r4, 0x0
    li r5, 0xf5
    li r6, 0x0
    bl fn_8012640C
    extsh r7, r3
    mr r3, r22
    add r0, r7, r24
    li r4, 0x0
    extsh r7, r0
    li r5, 0xf5
    li r6, 0x0
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x1(r3)
    cmplwi r0, 0x12
    bne @L_80226BA0
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0xf6
    li r6, 0x0
    bl fn_801254B4
    b @L_80226BC8
@L_80226BA0:
    mr r3, r27
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0xf6
    li r6, 0x0
    bl fn_801254B4
@L_80226BC8:
    mr r3, r22
    mr r4, r28
    rlwinm r5, r29, 0, 16, 31
    bl fn_801FE55C
@L_80226BD8:
    mr r3, r22
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80226C18
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 11, 11
    bne @L_80226C18
    mr r3, r22
    mr r7, r29
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_801254B4
@L_80226C18:
    mr r3, r26
    bl fn_8010C4A0
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_80226D24
    bge @L_80226DFC
    cmpwi r0, 0x1
    bge @L_80226C3C
    b @L_80226DFC
@L_80226C3C:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 11, 11
    bne @L_80226DFC
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0xdc
    beq @L_80226DFC
    mr r3, r22
    mr r7, r29
    li r4, 0x0
    li r5, 0x102
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r29
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x1(r3)
    cmplwi r0, 0x12
    bne @L_80226CDC
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r24, r0, 0, 16, 31
    li r4, 0x0
    mr r7, r24
    li r5, 0x103
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r24
    li r4, 0x0
    li r5, 0x11d
    li r6, 0x0
    bl fn_801254B4
    b @L_80226DFC
@L_80226CDC:
    mr r3, r27
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r24, r0, 0, 16, 31
    li r4, 0x0
    mr r7, r24
    li r5, 0x103
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r24
    li r4, 0x0
    li r5, 0x11d
    li r6, 0x0
    bl fn_801254B4
    b @L_80226DFC
@L_80226D24:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 11, 11
    bne @L_80226DFC
    mr r3, r22
    mr r7, r29
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r29
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x1(r3)
    cmplwi r0, 0x12
    bne @L_80226DB8
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r24, r0, 0, 16, 31
    li r4, 0x0
    mr r7, r24
    li r5, 0x105
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r24
    li r4, 0x0
    li r5, 0x11f
    li r6, 0x0
    bl fn_801254B4
    b @L_80226DFC
@L_80226DB8:
    mr r3, r27
    mr r4, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r22
    rlwinm r24, r0, 0, 16, 31
    li r4, 0x0
    mr r7, r24
    li r5, 0x105
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    mr r7, r24
    li r4, 0x0
    li r5, 0x11f
    li r6, 0x0
    bl fn_801254B4
@L_80226DFC:
    lwz r0, lbl_8047B618(r13)
    mr r3, r25
    mr r7, r29
    li r4, 0x0
    rlwinm r0, r0, 0, 12, 10
    li r5, 0x2e
    stw r0, lbl_8047B618(r13)
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r21
    mr r7, r23
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_801254B4
    mr r3, r22
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226E60
    mr r3, r22
    li r4, 0x83
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80226E60:
    mr r3, r22
    mr r4, r31
    li r5, 0x1
    bl fn_802656AC
    extsh r21, r23
    mr r20, r3
    bl fn_80102620
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80226E94
    mr r3, r22
    mr r4, r31
    li r5, 0x1
    bl fn_80265598
@L_80226E94:
    mr r3, r20
    mr r4, r21
    bl fn_80011CF0
    b @L_80226EEC
@L_80226EA4:
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    lbz r3, 0x1(r3)
    bl fn_801F025C
    li r4, 0x0
    mr r21, r3
    li r5, 0x11b
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80226EEC
    lis r4, 0x1
    mr r3, r21
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_801254B4
@L_80226EEC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r19, 0xc(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
