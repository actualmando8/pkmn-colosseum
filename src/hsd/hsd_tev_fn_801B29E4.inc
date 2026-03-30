    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r31, r4
    mr r30, r3
    beq @L_801B2C28
    lbz r4, 0x0(r31)
    lbz r3, -21763(r13)
    rlwinm r0, r4, 0, 31, 31
    rlwinm r30, r4, 0, 31, 31
    cmplw r3, r0
    beq @L_801B2A24
    mr r3, r30
    bl fn_800BCE30
    stb r30, -21763(r13)
@L_801B2A24:
    lbz r4, 0x0(r31)
    lbz r3, -21764(r13)
    rlwinm r0, r4, 31, 31, 31
    rlwinm r30, r4, 31, 31, 31
    cmplw r3, r0
    beq @L_801B2A48
    mr r3, r30
    bl fn_800BCE5C
    stb r30, -21764(r13)
@L_801B2A48:
    lbz r4, 0x0(r31)
    lbz r3, -21765(r13)
    rlwinm r0, r4, 30, 31, 31
    lbz r29, 0x3(r31)
    cmplw r3, r0
    rlwinm r30, r4, 30, 31, 31
    bne @L_801B2A74
    lbz r3, -21766(r13)
    rlwinm r0, r29, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2A88
@L_801B2A74:
    mr r3, r30
    mr r4, r29
    bl fn_800BD008
    stb r30, -21765(r13)
    stb r29, -21766(r13)
@L_801B2A88:
    lbz r29, 0x4(r31)
    lwz r0, -21720(r13)
    lbz r27, 0x7(r31)
    cmpw r0, r29
    lbz r28, 0x6(r31)
    lbz r30, 0x5(r31)
    bne @L_801B2AC8
    lwz r0, -21724(r13)
    cmpw r0, r30
    bne @L_801B2AC8
    lwz r0, -21728(r13)
    cmpw r0, r28
    bne @L_801B2AC8
    lwz r0, -21732(r13)
    cmpw r0, r27
    beq @L_801B2AEC
@L_801B2AC8:
    mr r3, r29
    mr r4, r30
    mr r5, r28
    mr r6, r27
    bl fn_800BCDDC
    stw r29, -21720(r13)
    stw r30, -21724(r13)
    stw r28, -21728(r13)
    stw r27, -21732(r13)
@L_801B2AEC:
    lbz r5, 0x0(r31)
    lbz r3, 0x0(r31)
    lbz r0, -21736(r13)
    rlwinm r28, r5, 28, 31, 31
    rlwinm r6, r3, 0, 26, 26
    lbz r29, 0x8(r31)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r5, 28, 31, 31
    neg r4, r6
    or r4, r4, r6
    cmplw r3, r0
    rlwinm r27, r4, 1, 31, 31
    bne @L_801B2B3C
    lwz r0, -21740(r13)
    cmpw r0, r29
    bne @L_801B2B3C
    lbz r3, -21744(r13)
    rlwinm r0, r27, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2B58
@L_801B2B3C:
    mr r3, r28
    mr r4, r29
    mr r5, r27
    bl fn_800BCE88
    stb r28, -21736(r13)
    stw r29, -21740(r13)
    stb r27, -21744(r13)
@L_801B2B58:
    lbz r4, 0x0(r31)
    lbz r3, -21767(r13)
    rlwinm r0, r4, 29, 31, 31
    rlwinm r27, r4, 29, 31, 31
    cmplw r3, r0
    beq @L_801B2B7C
    mr r3, r27
    bl fn_800BCEBC
    stb r27, -21767(r13)
@L_801B2B7C:
    lbz r27, 0x9(r31)
    lwz r0, -21748(r13)
    lbz r26, 0x2(r31)
    cmpw r0, r27
    lbz r30, 0xb(r31)
    lbz r29, 0xa(r31)
    lbz r28, 0x1(r31)
    bne @L_801B2BD4
    lbz r3, -21752(r13)
    rlwinm r0, r28, 0, 24, 31
    cmplw r3, r0
    bne @L_801B2BD4
    lwz r0, -21756(r13)
    cmpw r0, r29
    bne @L_801B2BD4
    lwz r0, -21760(r13)
    cmpw r0, r30
    bne @L_801B2BD4
    lbz r3, -21762(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2C00
@L_801B2BD4:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r26
    bl fn_800BC618
    stw r27, -21748(r13)
    stb r28, -21752(r13)
    stw r29, -21756(r13)
    stw r30, -21760(r13)
    stb r26, -21762(r13)
@L_801B2C00:
    lbz r4, 0x0(r31)
    lbz r3, -21768(r13)
    rlwinm r0, r4, 26, 31, 31
    rlwinm r26, r4, 26, 31, 31
    cmplw r3, r0
    beq @L_801B2F08
    mr r3, r26
    bl fn_800BCFDC
    stb r26, -21768(r13)
    b @L_801B2F08
@L_801B2C28:
    li r26, 0x1
    lbz r3, -21763(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2C48
    mr r3, r26
    bl fn_800BCE30
    stb r26, -21763(r13)
@L_801B2C48:
    li r26, 0x0
    lbz r3, -21764(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2C68
    mr r3, r26
    bl fn_800BCE5C
    stb r26, -21764(r13)
@L_801B2C68:
    li r26, 0x0
    lbz r3, -21765(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    bne @L_801B2C88
    lbz r0, -21766(r13)
    cmplwi r0, 0x0
    beq @L_801B2CA0
@L_801B2C88:
    mr r3, r26
    li r4, 0x0
    bl fn_800BD008
    li r0, 0x0
    stb r26, -21765(r13)
    stb r0, -21766(r13)
@L_801B2CA0:
    lwz r3, -21720(r13)
    rlwinm r0, r30, 2, 31, 31
    cmpw r3, r0
    bne @L_801B2CD4
    lwz r0, -21724(r13)
    cmpwi r0, 0x4
    bne @L_801B2CD4
    lwz r0, -21728(r13)
    cmpwi r0, 0x5
    bne @L_801B2CD4
    lwz r0, -21732(r13)
    cmpwi r0, 0xf
    beq @L_801B2D08
@L_801B2CD4:
    rlwinm r3, r30, 2, 31, 31
    li r4, 0x4
    li r5, 0x5
    li r6, 0xf
    bl fn_800BCDDC
    rlwinm r5, r30, 2, 31, 31
    li r4, 0x4
    li r3, 0x5
    li r0, 0xf
    stw r5, -21720(r13)
    stw r4, -21724(r13)
    stw r3, -21728(r13)
    stw r0, -21732(r13)
@L_801B2D08:
    rlwinm r3, r30, 3, 31, 31
    lbz r0, -21736(r13)
    xori r3, r3, 0x1
    li r31, 0x1
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r31, 0, 24, 31
    neg r4, r5
    or r4, r4, r5
    cmplw r3, r0
    rlwinm r26, r4, 1, 31, 31
    bne @L_801B2D64
    rlwinm. r0, r30, 0, 4, 4
    lwz r3, -21740(r13)
    li r0, 0x3
    beq @L_801B2D4C
    li r0, 0x7
@L_801B2D4C:
    cmpw r3, r0
    bne @L_801B2D64
    lbz r3, -21744(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2D9C
@L_801B2D64:
    rlwinm. r0, r30, 0, 4, 4
    mr r3, r31
    li r4, 0x3
    beq @L_801B2D78
    li r4, 0x7
@L_801B2D78:
    mr r5, r26
    bl fn_800BCE88
    rlwinm. r0, r30, 0, 4, 4
    stb r31, -21736(r13)
    li r0, 0x3
    beq @L_801B2D94
    li r0, 0x7
@L_801B2D94:
    stw r0, -21740(r13)
    stb r26, -21744(r13)
@L_801B2D9C:
    rlwinm. r0, r30, 0, 2, 2
    bne @L_801B2E4C
    rlwinm. r0, r30, 0, 1, 1
    beq @L_801B2E4C
    li r26, 0x0
    lbz r3, -21767(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2DCC
    mr r3, r26
    bl fn_800BCEBC
    stb r26, -21767(r13)
@L_801B2DCC:
    lwz r0, -21748(r13)
    cmpwi r0, 0x4
    bne @L_801B2E08
    lbz r0, -21752(r13)
    cmplwi r0, 0x0
    bne @L_801B2E08
    lwz r0, -21756(r13)
    cmpwi r0, 0x0
    bne @L_801B2E08
    lwz r0, -21760(r13)
    cmpwi r0, 0x4
    bne @L_801B2E08
    lbz r0, -21762(r13)
    cmplwi r0, 0x0
    beq @L_801B2EE8
@L_801B2E08:
    li r3, 0x4
    li r4, 0x0
    li r5, 0x0
    li r6, 0x4
    li r7, 0x0
    bl fn_800BC618
    li r6, 0x4
    li r5, 0x0
    li r4, 0x0
    li r3, 0x4
    li r0, 0x0
    stw r6, -21748(r13)
    stb r5, -21752(r13)
    stw r4, -21756(r13)
    stw r3, -21760(r13)
    stb r0, -21762(r13)
    b @L_801B2EE8
@L_801B2E4C:
    li r26, 0x1
    lbz r3, -21767(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2E6C
    mr r3, r26
    bl fn_800BCEBC
    stb r26, -21767(r13)
@L_801B2E6C:
    lwz r0, -21748(r13)
    cmpwi r0, 0x7
    bne @L_801B2EA8
    lbz r0, -21752(r13)
    cmplwi r0, 0x0
    bne @L_801B2EA8
    lwz r0, -21756(r13)
    cmpwi r0, 0x0
    bne @L_801B2EA8
    lwz r0, -21760(r13)
    cmpwi r0, 0x7
    bne @L_801B2EA8
    lbz r0, -21762(r13)
    cmplwi r0, 0x0
    beq @L_801B2EE8
@L_801B2EA8:
    li r3, 0x7
    li r4, 0x0
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    bl fn_800BC618
    li r6, 0x7
    li r5, 0x0
    li r4, 0x0
    li r3, 0x7
    li r0, 0x0
    stw r6, -21748(r13)
    stb r5, -21752(r13)
    stw r4, -21756(r13)
    stw r3, -21760(r13)
    stb r0, -21762(r13)
@L_801B2EE8:
    li r26, 0x0
    lbz r3, -21768(r13)
    rlwinm r0, r26, 0, 24, 31
    cmplw r3, r0
    beq @L_801B2F08
    mr r3, r26
    bl fn_800BCFDC
    stb r26, -21768(r13)
@L_801B2F08:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
