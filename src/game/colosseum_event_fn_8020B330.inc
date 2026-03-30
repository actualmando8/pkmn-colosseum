    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r31, r3
    bl fn_8020D920
    bl fn_8020D814
    rlwinm r30, r3, 0, 16, 31
    li r3, 0xb
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x9
    mr r25, r0
    mr r4, r25
    bl fn_801F025C
    li r4, 0x0
    mr r24, r3
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r26, r3, 0, 16, 31
    mr r3, r24
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr r29, r3
    mr r4, r26
    li r3, 0x0
    li r5, 0x8
    li r6, 0x1
    bl fn_801FB1C0
    mr r28, r3
    bl fn_800896B8
    cmplw r26, r3
    bne @L_8020B3EC
    bl fn_800896C0
    cmplwi r3, 0x0
    bne @L_8020B3D8
    li r23, 0x0
    b @L_8020B458
@L_8020B3D8:
    mr r4, r3
    li r3, 0x24
    bl fn_80132A38
    li r23, 0x7531
    b @L_8020B458
@L_8020B3EC:
    mr r3, r24
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_8020B424
    mr r4, r26
    li r3, 0x0
    li r5, 0x8
    li r6, 0x2
    bl fn_801FB1C0
    mr r23, r3
    b @L_8020B458
@L_8020B424:
    mr r4, r26
    li r3, 0x0
    li r5, 0x8
    li r6, 0x3
    bl fn_801FB1C0
    mr. r23, r3
    bne @L_8020B458
    mr r4, r26
    li r3, 0x0
    li r5, 0x8
    li r6, 0x2
    bl fn_801FB1C0
    mr r23, r3
@L_8020B458:
    mr r3, r24
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r24
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    mr r3, r25
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    mr r3, r24
    bl fn_801F8100
    mr r4, r3
    li r3, 0x25
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B654
    cmplwi r30, 0x2
    bne @L_8020B57C
    cmplwi r23, 0x0
    beq @L_8020B4F4
    mr r3, r29
    li r4, 0x5a
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    bl fn_801C2D54
    mr r27, r3
@L_8020B4F4:
    li r3, 0x3f5
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    li r3, 0x5d
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x766c
    bl fn_802624CC
    bl fn_8026246C
    cmplwi r23, 0x0
    beq @L_8020B654
    mr r3, r29
    li r4, 0x5a
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r23
    bl fn_80262490
@L_8020B53C:
    mr r3, r29
    li r4, 0x5a
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020B55C
    bl fn_800F0308
    b @L_8020B53C
@L_8020B55C:
    mr r3, r27
    bl fn_801EF8F4
    bl fn_8026246C
    mr r3, r29
    li r4, 0x5a
    li r5, 0x4
    bl fn_801DA8C4
    b @L_8020B654
@L_8020B57C:
    cmplwi r30, 0x3
    bne @L_8020B620
    cmplwi r28, 0x0
    beq @L_8020B5A8
    mr r3, r29
    li r4, 0x59
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    bl fn_801C2D54
    mr r27, r3
@L_8020B5A8:
    li r3, 0x7547
    bl fn_802624CC
    bl fn_8026246C
    cmplwi r28, 0x0
    beq @L_8020B610
    mr r3, r29
    li r4, 0x59
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r28
    bl fn_80262490
@L_8020B5D4:
    mr r3, r29
    li r4, 0x59
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020B5F4
    bl fn_800F0308
    b @L_8020B5D4
@L_8020B5F4:
    mr r3, r27
    bl fn_801EF8F4
    bl fn_8026246C
    mr r3, r29
    li r4, 0x59
    li r5, 0x4
    bl fn_801DA8C4
@L_8020B610:
    li r3, 0x7548
    bl fn_802624CC
    bl fn_8026246C
    b @L_8020B654
@L_8020B620:
    subi r0, r30, 0x4
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x1
    ble @L_8020B654
    cmplwi r30, 0x7
    beq @L_8020B640
    cmplwi r30, 0x6
    bne @L_8020B654
@L_8020B640:
    li r3, 0x7640
    bl fn_802624CC
    li r3, 0x40
    bl fn_801F000C
    bl fn_8026246C
@L_8020B654:
    bl fn_801EF634
    mr r4, r3
    li r3, 0x0
    bl fn_801F1DBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B6B8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x25
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B6B8
    lis r4, -32712
    mr r3, r31
    addi r4, r4, -30719
    li r5, 0x0
    bl fn_80211B94
    lis r4, -32712
    mr r3, r31
    addi r4, r4, -30705
    li r5, 0x0
    bl fn_80211B94
@L_8020B6B8:
    bl fn_801EF2D4
    lmw r23, 0xc(r1)
    li r3, 0x1
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
