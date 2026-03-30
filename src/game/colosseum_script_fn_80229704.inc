    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x34
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    mr r25, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 24, 31
    bl fn_80077B60
    mr r30, r3
    bl fn_80077B3C
    cmplwi r24, 0x1
    bne @L_8022991C
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x8
    bne @L_80229834
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022991C
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
    b @L_80229814
@L_802297A8:
    mr r3, r25
    rlwinm r4, r29, 0, 16, 31
    bl fn_801F7258
    mr. r28, r3
    beq @L_80229810
    li r30, 0x0
    b @L_80229804
@L_802297C4:
    mr r3, r28
    mr r4, r30
    bl fn_801F986C
    mr. r24, r3
    beq @L_80229800
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80229800
    mr r3, r24
    mr r4, r31
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229800
    addi r27, r27, 0x1
@L_80229800:
    addi r30, r30, 0x1
@L_80229804:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_802297C4
@L_80229810:
    addi r29, r29, 0x1
@L_80229814:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r26
    blt @L_802297A8
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_8022991C
    li r3, 0x1
    b @L_80229920
@L_80229834:
    cmplwi r0, 0x7
    bne @L_8022991C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022991C
    mr r4, r25
    li r27, 0x0
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    li r25, 0x0
    b @L_802298FC
@L_80229890:
    mr r3, r29
    rlwinm r4, r25, 0, 16, 31
    bl fn_801F7258
    mr. r26, r3
    beq @L_802298F8
    li r24, 0x0
    b @L_802298EC
@L_802298AC:
    mr r3, r26
    mr r4, r24
    bl fn_801F986C
    mr. r30, r3
    beq @L_802298E8
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802298E8
    mr r3, r30
    li r4, 0x7
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802298E8
    addi r27, r27, 0x1
@L_802298E8:
    addi r24, r24, 0x1
@L_802298EC:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_802298AC
@L_802298F8:
    addi r25, r25, 0x1
@L_802298FC:
    rlwinm r0, r25, 0, 16, 31
    cmplw r0, r28
    blt @L_80229890
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_8022991C
    li r3, 0x1
    b @L_80229920
@L_8022991C:
    li r3, 0x0
@L_80229920:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
