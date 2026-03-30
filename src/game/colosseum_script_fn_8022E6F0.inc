    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r31, r3
    mr r23, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    cmplwi r31, 0x0
    beq @L_8022EB88
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022EB88
    mr r3, r31
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022EB88
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r29, r0
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E834
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EB88
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x0
    bl fn_8011F910
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E7D0
    mr r3, r31
    li r4, 0xc5
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022E7D0:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    li r5, 0x1
    bl fn_802653FC
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EB88
    mr r3, r31
    li r4, 0xd1
    li r5, 0x0
    bl fn_801FE710
    b @L_8022EB88
@L_8022E834:
    rlwinm. r0, r23, 0, 24, 31
    bne @L_8022EB88
    li r3, 0x0
    bl fn_801F2A7C
    li r4, 0x0
    mr r25, r3
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    mr r3, r25
    mr r26, r0
    bl fn_801F7B70
    mr r30, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EB88
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EB88
    cmplwi r26, 0x0
    beq @L_8022EB88
    li r3, 0x0
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
    cmplwi r24, 0x11
    beq @L_8022E8D8
    li r23, 0x0
    b @L_8022E8FC
@L_8022E8D8:
    mr r3, r31
    li r23, 0x3
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    mr r3, r25
    li r4, 0x0
    bl fn_801FBA24
@L_8022E8FC:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r28, r3
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x32
    bl fn_801DE418
    li r4, 0xa0
    mr r27, r3
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x0
    bl fn_80208404
    mr r3, r26
    mr r4, r27
    bl fn_801C3F10
    bl fn_801C3430
    mr r3, r27
    li r4, 0xa0
    li r5, 0x4
    bl fn_801DA9E8
    li r3, 0x0
    bl fn_801EF7C4
    mr r3, r27
    li r4, 0x1
    bl fn_801DA4E8
@L_8022E990:
    mr r3, r27
    li r4, 0xa0
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022E9B0
    bl fn_800F0308
    b @L_8022E990
@L_8022E9B0:
    mr r3, r27
    mr r4, r26
    bl fn_801C3F10
    bl fn_801C3430
    li r3, 0x1
    bl fn_801EF7C4
    mr r3, r27
    li r4, 0x0
    bl fn_801DA4E8
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x1
    bl fn_80208404
    cmplwi r24, 0x11
    bne @L_8022EAA8
    li r3, 0x7729
    bl fn_802624CC
    bl fn_8026246C
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x4
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x6
    bl fn_80208554
    mr r3, r25
    li r4, 0x1
    bl fn_801FBA24
    mr r3, r25
    li r4, 0x2
    bl fn_801FBA24
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x1
    bl fn_80208404
    li r3, 0x772a
    bl fn_802624CC
    rlwinm. r0, r30, 0, 24, 31
    bne @L_8022EA8C
    bl fn_8026246C
    li r3, 0x40
    bl fn_801F000C
    li r3, 0x772b
    bl fn_802624CC
@L_8022EA8C:
    bl fn_8026246C
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x4
    bl fn_80208404
    b @L_8022EAE4
@L_8022EAA8:
    li r3, 0x9a0
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8022EAC4
    li r3, 0x7717
    bl fn_802624CC
    b @L_8022EACC
@L_8022EAC4:
    li r3, 0x770a
    bl fn_802624CC
@L_8022EACC:
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
@L_8022EAE4:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022EB20
    mr r3, r31
    li r4, 0xd1
    li r5, 0x0
    bl fn_801FE710
@L_8022EB20:
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    mr r5, r23
    li r4, 0x0
    li r6, 0x3
    bl fn_80208404
    cmplwi r24, 0x11
    bne @L_8022EB70
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    bl fn_802086E8
    mr r3, r25
    li r4, 0x3
    bl fn_801FBA24
@L_8022EB70:
    mr r3, r27
    li r4, 0xa0
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r27
    bl fn_801DB100
@L_8022EB88:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
