    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r5, lbl_80274AA0@ha
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r30, r3
    addi r29, r5, lbl_80274AA0@l
    mr r31, r4
    b @L_801A0B78
    @L_801A0768:
    cmplwi r30, 0x0
    beq @L_801A0944
    cmplwi r31, 0x0
    beq @L_801A0944
    lwz r3, 0x80(r30)
    lwz r4, 0x3c(r31)
    bl fn_801AEBE4
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A0924
    lwz r28, 0x10(r30)
    cmplwi r28, 0x0
    beq @L_801A08C0
    lis r3, 0x1
    lhz r4, 0x4(r28)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A07C0
    b @L_801A07D8
    @L_801A07C0:
    lhz r0, 0x4(r28)
    lhz r3, 0x4(r28)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r28)
    srwi r3, r4, 5
    @L_801A07D8:
    cmpwi r3, 0x0
    beq @L_801A08C0
    lhz r3, 0x6(r28)
    subic. r0, r3, 0x1
    bge @L_801A0820
    cmplwi r28, 0x0
    beq @L_801A08C0
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_801A08C0
    @L_801A0820:
    lhz r3, 0x6(r28)
    addi r0, r3, 0x1
    sth r0, 0x6(r28)
    lhz r0, 0x6(r28)
    cmplwi r0, 0x0
    bne @L_801A0848
    addi r3, r29, 0x54
    addi r5, r29, 0x60
    li r4, 0x9e
    bl fn_80196E10
    @L_801A0848:
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lhz r0, 0x6(r28)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0870
    b @L_801A0888
    @L_801A0870:
    lhz r3, 0x6(r28)
    subi r0, r3, 0x1
    sth r0, 0x6(r28)
    lhz r0, 0x6(r28)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_801A0888:
    cmpwi r0, 0x0
    beq @L_801A08C0
    cmplwi r28, 0x0
    beq @L_801A08C0
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A08C0:
    lwz r4, 0x8(r31)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x10(r30)
    lwz r0, 0x10(r30)
    cmplwi r0, 0x0
    bne @L_801A08F0
    addi r5, r29, 0x210
    la r3, lbl_8047DB20(r2)
    li r4, 0x45f
    bl fn_80196E10
    @L_801A08F0:
    lwz r4, 0x10(r30)
    cmplwi r4, 0x0
    beq @L_801A0924
    lhz r3, 0x4(r4)
    addi r0, r3, 0x1
    sth r0, 0x4(r4)
    lhz r0, 0x4(r4)
    cmplwi r0, 0xffff
    bne @L_801A0924
    addi r3, r29, 0x54
    addi r5, r29, 0xc4
    li r4, 0x5d
    bl fn_80196E10
    @L_801A0924:
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0944
    lwz r3, 0x18(r30)
    lwz r4, 0x10(r31)
    bl fn_801991F8
    @L_801A0944:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A0B70
    lwz r27, 0x8(r31)
    lwz r28, 0x10(r30)
    b @L_801A0B60
    @L_801A095C:
    cmplwi r28, 0x0
    beq @L_801A0A50
    cmplwi r27, 0x0
    beq @L_801A0A50
    lwz r3, 0x80(r28)
    lwz r4, 0x3c(r27)
    bl fn_801AEBE4
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A0A30
    lwz r25, 0x10(r28)
    cmplwi r25, 0x0
    beq @L_801A09F0
    mr r3, r25
    bl fn_801A0D48
    cmpwi r3, 0x0
    beq @L_801A09F0
    mr r3, r25
    bl fn_801A0D3C
    cmpwi r3, 0x0
    bge @L_801A09BC
    mr r3, r25
    bl fn_801A0CE8
    b @L_801A09F0
    @L_801A09BC:
    mr r3, r25
    bl fn_801A0C9C
    lwz r4, 0x0(r25)
    mr r3, r25
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    mr r3, r25
    bl fn_801A0C68
    cmpwi r3, 0x0
    beq @L_801A09F0
    mr r3, r25
    bl fn_801A0CE8
    @L_801A09F0:
    lwz r4, 0x8(r27)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x10(r28)
    lwz r0, 0x10(r28)
    cmplwi r0, 0x0
    bne @L_801A0A20
    addi r5, r29, 0x210
    la r3, lbl_8047DB20(r2)
    li r4, 0x45f
    bl fn_80196E10
    @L_801A0A20:
    lwz r3, 0x10(r28)
    cmplwi r3, 0x0
    beq @L_801A0A30
    bl fn_801A0C1C
    @L_801A0A30:
    lwz r0, 0x14(r28)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0A50
    lwz r3, 0x18(r28)
    lwz r4, 0x10(r27)
    bl fn_801991F8
    @L_801A0A50:
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A0B58
    lwz r25, 0x8(r27)
    lwz r26, 0x10(r28)
    b @L_801A0B48
    @L_801A0A68:
    cmplwi r26, 0x0
    beq @L_801A0AEC
    cmplwi r25, 0x0
    beq @L_801A0AEC
    lwz r3, 0x80(r26)
    lwz r4, 0x3c(r25)
    bl fn_801AEBE4
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A0ACC
    lwz r3, 0x10(r26)
    bl fn_801A05EC
    lwz r3, 0x8(r25)
    li r4, 0x0
    bl fn_801A0BF0
    stw r3, 0x10(r26)
    lwz r0, 0x10(r26)
    cmplwi r0, 0x0
    bne @L_801A0AC4
    addi r5, r29, 0x210
    la r3, lbl_8047DB20(r2)
    li r4, 0x45f
    bl fn_80196E10
    @L_801A0AC4:
    lwz r3, 0x10(r26)
    bl fn_801A0B9C
    @L_801A0ACC:
    lwz r0, 0x14(r26)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0AEC
    lwz r3, 0x18(r26)
    lwz r4, 0x10(r25)
    bl fn_801991F8
    @L_801A0AEC:
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A0B40
    lwz r24, 0x10(r26)
    lwz r23, 0x8(r25)
    b @L_801A0B30
    @L_801A0B04:
    mr r3, r24
    mr r4, r23
    bl fn_801A0D94
    lwz r0, 0x14(r24)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A0B28
    lwz r3, 0x10(r24)
    lwz r4, 0x8(r23)
    bl fn_801A0744
    @L_801A0B28:
    lwz r24, 0x8(r24)
    lwz r23, 0xc(r23)
    @L_801A0B30:
    cmplwi r24, 0x0
    beq @L_801A0B40
    cmplwi r23, 0x0
    bne @L_801A0B04
    @L_801A0B40:
    lwz r26, 0x8(r26)
    lwz r25, 0xc(r25)
    @L_801A0B48:
    cmplwi r26, 0x0
    beq @L_801A0B58
    cmplwi r25, 0x0
    bne @L_801A0A68
    @L_801A0B58:
    lwz r28, 0x8(r28)
    lwz r27, 0xc(r27)
    @L_801A0B60:
    cmplwi r28, 0x0
    beq @L_801A0B70
    cmplwi r27, 0x0
    bne @L_801A095C
    @L_801A0B70:
    lwz r30, 0x8(r30)
    lwz r31, 0xc(r31)
    @L_801A0B78:
    cmplwi r30, 0x0
    beq @L_801A0B88
    cmplwi r31, 0x0
    bne @L_801A0768
    @L_801A0B88:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
