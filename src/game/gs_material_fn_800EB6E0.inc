    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x48(r1)
    fmr f31, f1
    stmw r16, 0x8(r1)
    mr. r18, r4
    mr r28, r3
    mr r17, r5
    mr r16, r6
    beq @L_800EB8EC
    cmplwi r17, 0x0
    beq @L_800EB8EC
    cmplwi r16, 0x0
    bne @L_800EB720
    b @L_800EB8EC
    @L_800EB720:
    bl fn_800EB904
    lwz r0, 0x14(r18)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EB8EC
    lwz r31, 0x10(r18)
    lwz r30, 0x10(r17)
    lwz r29, 0x10(r16)
    b @L_800EB8E4
    @L_800EB740:
    cmplwi r31, 0x0
    beq @L_800EB8D8
    cmplwi r30, 0x0
    beq @L_800EB8D8
    cmplwi r29, 0x0
    beq @L_800EB8D8
    fmr f1, f31
    mr r3, r28
    mr r4, r31
    mr r5, r30
    mr r6, r29
    bl fn_800EB904
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EB8D8
    lwz r25, 0x10(r31)
    lwz r26, 0x10(r30)
    lwz r27, 0x10(r29)
    b @L_800EB8D0
    @L_800EB78C:
    cmplwi r25, 0x0
    beq @L_800EB8C4
    cmplwi r26, 0x0
    beq @L_800EB8C4
    cmplwi r27, 0x0
    beq @L_800EB8C4
    fmr f1, f31
    mr r3, r28
    mr r4, r25
    mr r5, r26
    mr r6, r27
    bl fn_800EB904
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EB8C4
    lwz r22, 0x10(r25)
    lwz r23, 0x10(r26)
    lwz r24, 0x10(r27)
    b @L_800EB8BC
    @L_800EB7D8:
    cmplwi r22, 0x0
    beq @L_800EB8B0
    cmplwi r23, 0x0
    beq @L_800EB8B0
    cmplwi r24, 0x0
    beq @L_800EB8B0
    fmr f1, f31
    mr r3, r28
    mr r4, r22
    mr r5, r23
    mr r6, r24
    bl fn_800EB904
    lwz r0, 0x14(r22)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EB8B0
    lwz r19, 0x10(r22)
    lwz r20, 0x10(r23)
    lwz r21, 0x10(r24)
    b @L_800EB8A8
    @L_800EB824:
    cmplwi r19, 0x0
    beq @L_800EB89C
    cmplwi r20, 0x0
    beq @L_800EB89C
    cmplwi r21, 0x0
    beq @L_800EB89C
    fmr f1, f31
    mr r3, r28
    mr r4, r19
    mr r5, r20
    mr r6, r21
    bl fn_800EB904
    lwz r0, 0x14(r19)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EB89C
    lwz r16, 0x10(r19)
    lwz r17, 0x10(r20)
    lwz r18, 0x10(r21)
    b @L_800EB894
    @L_800EB870:
    fmr f1, f31
    mr r3, r28
    mr r4, r16
    mr r5, r17
    mr r6, r18
    bl fn_800EB6E0
    lwz r16, 0x8(r16)
    lwz r17, 0x8(r17)
    lwz r18, 0x8(r18)
    @L_800EB894:
    cmplwi r16, 0x0
    bne @L_800EB870
    @L_800EB89C:
    lwz r19, 0x8(r19)
    lwz r20, 0x8(r20)
    lwz r21, 0x8(r21)
    @L_800EB8A8:
    cmplwi r19, 0x0
    bne @L_800EB824
    @L_800EB8B0:
    lwz r22, 0x8(r22)
    lwz r23, 0x8(r23)
    lwz r24, 0x8(r24)
    @L_800EB8BC:
    cmplwi r22, 0x0
    bne @L_800EB7D8
    @L_800EB8C4:
    lwz r25, 0x8(r25)
    lwz r26, 0x8(r26)
    lwz r27, 0x8(r27)
    @L_800EB8D0:
    cmplwi r25, 0x0
    bne @L_800EB78C
    @L_800EB8D8:
    lwz r31, 0x8(r31)
    lwz r30, 0x8(r30)
    lwz r29, 0x8(r29)
    @L_800EB8E4:
    cmplwi r31, 0x0
    bne @L_800EB740
    @L_800EB8EC:
    lfd f31, 0x48(r1)
    lmw r16, 0x8(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
