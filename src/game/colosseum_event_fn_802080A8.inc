    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r24, r7
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r26, r3
    beq @L_802083F0
    rlwinm. r0, r24, 0, 24, 31
    bne @L_8020816C
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208124
    li r4, 0xa3
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
@L_80208124:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208144
    mr r3, r26
    li r4, 0x9f
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
@L_80208144:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802083F0
    rlwinm. r0, r29, 0, 24, 31
    bne @L_802083F0
    mr r3, r26
    li r4, 0x57
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_802083F0
@L_8020816C:
    cmplwi r0, 0x1
    bne @L_80208338
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208228
    li r4, 0xa3
    li r5, 0x4
    bl fn_801DA9E8
    cmplwi r27, 0x0
    bne @L_8020819C
    li r3, 0x0
    b @L_802081D0
@L_8020819C:
    mr r3, r27
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802081C0
    li r3, 0x0
    b @L_802081D0
@L_802081C0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802081D0:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x61
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    mr r3, r30
    bl fn_802624CC
    rlwinm. r0, r29, 0, 24, 31
    bne @L_80208228
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
@L_80208228:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020830C
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208260
@L_80208240:
    mr r3, r26
    li r4, 0xa3
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208260
    bl fn_800F0308
    b @L_80208240
@L_80208260:
    mr r3, r26
    li r4, 0x9f
    li r5, 0x4
    bl fn_801DA9E8
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802082FC
    cmplwi r27, 0x0
    bne @L_80208288
    li r3, 0x0
    b @L_802082BC
@L_80208288:
    mr r3, r27
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802082AC
    li r3, 0x0
    b @L_802082BC
@L_802082AC:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802082BC:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x61
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    mr r3, r30
    bl fn_802624CC
@L_802082FC:
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
@L_8020830C:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802083F0
    rlwinm. r0, r29, 0, 24, 31
    bne @L_802083F0
    mr r3, r26
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r30
    bl fn_802624CC
    b @L_802083F0
@L_80208338:
    cmplwi r0, 0x2
    bne @L_80208394
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208350
    li r25, 0xa3
@L_80208350:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208360
    li r25, 0x9f
@L_80208360:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_80208374
    rlwinm. r0, r29, 0, 24, 31
    bne @L_80208374
    li r25, 0x57
@L_80208374:
    mr r3, r26
    mr r4, r25
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802083F0
    bl fn_800F0308
    b @L_80208374
@L_80208394:
    cmplwi r0, 0x3
    bne @L_802083F0
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802083B4
    li r4, 0xa3
    li r5, 0x4
    bl fn_801DA8C4
@L_802083B4:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802083D0
    mr r3, r26
    li r4, 0x9f
    li r5, 0x4
    bl fn_801DA8C4
@L_802083D0:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802083F0
    rlwinm. r0, r29, 0, 24, 31
    bne @L_802083F0
    mr r3, r26
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA8C4
@L_802083F0:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
