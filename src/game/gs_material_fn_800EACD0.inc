    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    lis r9, lbl_804016D0@ha
    stw r0, 0x84(r1)
    stmw r22, 0x58(r1)
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r27, r7
    mr r28, r8
    lwz r29, 0xc(r3)
    addi r3, r9, lbl_804016D0@l
    addi r30, r3, 0x30
    b @L_800EAFC8
    @L_800EAD08:
    lhz r3, 0xc(r29)
    rlwinm. r0, r3, 0, 20, 20
    bne @L_800EAFC4
    clrlwi. r0, r26, 24
    beq @L_800EAF38
    rlwinm r0, r3, 0, 18, 19
    cmpwi r0, 0x1000
    beq @L_800EAECC
    bge @L_800EAD38
    cmpwi r0, 0x0
    beq @L_800EAD44
    b @L_800EAF38
    @L_800EAD38:
    cmpwi r0, 0x2000
    beq @L_800EAF20
    b @L_800EAF38
    @L_800EAD44:
    lwz r0, 0x14(r29)
    cmplwi r0, 0x0
    bne @L_800EADA4
    bl fn_8019F01C
    mr r31, r3
    addi r4, r1, 0x18
    addi r5, r1, 0x1c
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x18(r1)
    cmplw r0, r31
    bne @L_800EAD80
    lwz r0, 0x1c(r1)
    cmplwi r0, 0x1
    beq @L_800EAF38
    @L_800EAD80:
    mr r4, r31
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    lis r3, lbl_804016D0@ha
    mr r4, r25
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    b @L_800EAF38
    @L_800EADA4:
    li r23, 0x0
    mr r31, r23
    bl fn_8019F01C
    mr r22, r3
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x10(r1)
    cmplw r0, r22
    beq @L_800EADE0
    lwz r0, 0x14(r1)
    cmplwi r0, 0x1
    beq @L_800EADE0
    li r23, 0x1
    @L_800EADE0:
    mr r4, r22
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    li r3, 0x1
    bl fn_801AB538
    lwz r0, 0x10(r1)
    lwz r4, 0x14(r29)
    cmplw r0, r4
    beq @L_800EAE20
    lwz r0, 0x14(r1)
    cmplwi r0, 0x1
    beq @L_800EAE20
    li r31, 0x1
    @L_800EAE20:
    li r3, 0x1
    li r5, 0x1
    bl fn_801AB5F8
    clrlwi. r0, r23, 24
    bne @L_800EAE3C
    clrlwi. r0, r31, 24
    beq @L_800EAF38
    @L_800EAE3C:
    clrlwi. r0, r23, 24
    beq @L_800EAE54
    lis r3, lbl_804016D0@ha
    mr r4, r25
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    @L_800EAE54:
    clrlwi. r0, r31, 24
    beq @L_800EAF38
    lwz r22, 0x14(r29)
    cmplwi r22, 0x0
    beq @L_800EAEA8
    bne @L_800EAE7C
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EAE7C:
    lwz r4, 0x14(r22)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EAE98
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EAE98
    li r3, 0x1
    @L_800EAE98:
    cmpwi r3, 0x0
    beq @L_800EAEA8
    mr r3, r22
    bl fn_8019D9DC
    @L_800EAEA8:
    lwz r4, 0x14(r29)
    mr r3, r24
    addi r5, r1, 0x20
    addi r4, r4, 0x44
    bl fn_800A2D98
    mr r3, r30
    addi r4, r1, 0x20
    bl fn_800E0628
    b @L_800EAF38
    @L_800EAECC:
    bl fn_8019F01C
    mr r22, r3
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x8(r1)
    cmplw r0, r22
    bne @L_800EAEFC
    lwz r0, 0xc(r1)
    cmplwi r0, 0x1
    beq @L_800EAF38
    @L_800EAEFC:
    mr r4, r22
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    lis r3, lbl_804016D0@ha
    mr r4, r25
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    b @L_800EAF38
    @L_800EAF20:
    lis r4, lbl_804016D0@ha
    mr r3, r29
    addi r6, r4, lbl_804016D0@l
    mr r5, r25
    mr r4, r24
    bl fn_800EAFE4
    @L_800EAF38:
    lhz r0, 0xc(r29)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    bne @L_800EAF6C
    mr r12, r27
    mr r3, r29
    mr r4, r24
    mr r5, r25
    mr r7, r28
    li r6, 0x0
    mtctr r12
    bctrl
    b @L_800EAFC4
    @L_800EAF6C:
    cmplwi r27, 0x0
    beq @L_800EAFC4
    clrlwi. r0, r26, 24
    beq @L_800EAFA4
    lis r3, lbl_804016D0@ha
    mr r12, r27
    addi r6, r3, lbl_804016D0@l
    mr r4, r24
    mr r3, r29
    mr r5, r25
    mr r7, r28
    mtctr r12
    bctrl
    b @L_800EAFC4
    @L_800EAFA4:
    mr r12, r27
    mr r3, r29
    mr r4, r24
    mr r5, r25
    mr r7, r28
    li r6, 0x0
    mtctr r12
    bctrl
    @L_800EAFC4:
    lwz r29, 0x4(r29)
    @L_800EAFC8:
    cmplwi r29, 0x0
    bne @L_800EAD08
    lmw r22, 0x58(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
