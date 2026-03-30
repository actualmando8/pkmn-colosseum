    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r27, r3
    mr r26, r4
    mr r25, r5
    li r3, 0x0
    li r4, 0x0
    li r5, 0x34
    bl fn_801F54A4
    rlwinm r24, r3, 0, 24, 31
    bl fn_80077B18
    mr r28, r3
    bl fn_80077AD0
    mr r29, r3
    bl fn_80077AAC
    mr r30, r3
    bl fn_80077B60
    mr r31, r3
    bl fn_80077B3C
    cmplwi r24, 0x1
    bne @L_80229B58
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x11d
    bne @L_802299B4
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80229B58
    li r3, 0x1
    b @L_80229B5C
@L_802299B4:
    cmplwi r0, 0xc3
    beq @L_802299C4
    cmplwi r0, 0xc2
    bne @L_80229A34
@L_802299C4:
    mr r4, r26
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r24, r0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r25, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r5, r3, 0, 16, 31
    mr r3, r24
    mr r4, r25
    bl fn_801F6FD4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bgt @L_80229B58
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80229B58
    li r3, 0x1
    b @L_80229B5C
@L_80229A34:
    cmplwi r0, 0x52
    beq @L_80229A44
    cmplwi r0, 0x31
    bne @L_80229A58
@L_80229A44:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80229B58
    li r3, 0x1
    b @L_80229B5C
@L_80229A58:
    mr r4, r27
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80229B58
    mr r4, r25
    li r27, 0x0
    li r3, 0x2
    bl fn_801F025C
    mr r25, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    li r29, 0x0
    b @L_80229B2C
@L_80229AC0:
    mr r3, r25
    rlwinm r4, r29, 0, 16, 31
    bl fn_801F7258
    mr. r30, r3
    beq @L_80229B28
    li r28, 0x0
    b @L_80229B1C
@L_80229ADC:
    mr r3, r30
    mr r4, r28
    bl fn_801F986C
    mr. r24, r3
    beq @L_80229B18
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80229B18
    mr r3, r24
    li r4, 0x8
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229B18
    addi r27, r27, 0x1
@L_80229B18:
    addi r28, r28, 0x1
@L_80229B1C:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80229ADC
@L_80229B28:
    addi r29, r29, 0x1
@L_80229B2C:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r26
    blt @L_80229AC0
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80229B58
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_80229B58
    li r3, 0x1
    b @L_80229B5C
@L_80229B58:
    li r3, 0x0
@L_80229B5C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
