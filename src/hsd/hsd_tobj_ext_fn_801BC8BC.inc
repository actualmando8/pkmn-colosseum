    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    li r0, -0x1
    stmw r23, 0x1c(r1)
    mr r27, r8
    mr r23, r3
    mr r24, r4
    mr r25, r6
    mr r26, r7
    li r30, 0x1
    li r29, 0x5
    lwz r8, 0x4c(r3)
    li r3, -0x1
    rlwinm r8, r8, 0, 23, 27
    stw r3, 0x14(r1)
    and r5, r5, r8
    neg r3, r5
    stw r0, 0x10(r1)
    or r0, r3, r5
    mr r3, r27
    rlwinm r28, r0, 1, 31, 31
    bl fn_801B707C
    lwz r0, 0xa8(r23)
    mr r31, r3
    cmplwi r0, 0x0
    beq @L_801BC954
    lwz r3, 0xa8(r23)
    lwz r0, 0x1c(r3)
    rlwinm. r0, r0, 0, 0, 1
    beq @L_801BC954
    mr r3, r24
    mr r4, r23
    mr r7, r27
    mr r8, r28
    addi r5, r1, 0x14
    addi r6, r1, 0x10
    bl fn_801BCF30
@L_801BC954:
    mr r3, r31
    mr r4, r23
    li r5, 0xff
    bl fn_801B5E40
    lwz r3, 0x4c(r23)
    lis r0, 0x4
    rlwinm r3, r3, 0, 12, 15
    cmpw r3, r0
    beq @L_801BCAEC
    bge @L_801BC9B8
    lis r0, 0x2
    cmpw r3, r0
    beq @L_801BCA40
    bge @L_801BC9A8
    lis r0, 0x1
    cmpw r3, r0
    beq @L_801BC9F8
    bge @L_801BCC54
    cmpwi r3, 0x0
    beq @L_801BCB7C
    b @L_801BCC54
@L_801BC9A8:
    lis r0, 0x3
    cmpw r3, r0
    beq @L_801BCA88
    b @L_801BCC54
@L_801BC9B8:
    lis r0, 0x7
    cmpw r3, r0
    beq @L_801BCBC4
    bge @L_801BC9E8
    lis r0, 0x6
    cmpw r3, r0
    beq @L_801BCB7C
    bge @L_801BCC54
    lis r0, 0x5
    cmpw r3, r0
    beq @L_801BCB34
    b @L_801BCC54
@L_801BC9E8:
    lis r0, 0x8
    cmpw r3, r0
    beq @L_801BCC0C
    b @L_801BCC54
@L_801BC9F8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r6, r30
    mr r8, r29
    li r4, 0x1
    lwz r5, 0x0(r25)
    li r10, 0x7
    lwz r7, 0x14(r1)
    lwz r9, 0x10(r1)
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCA40:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r6, r30
    mr r8, r30
    li r4, 0x1
    lwz r5, 0x0(r25)
    li r10, 0x7
    lwz r7, 0x14(r1)
    lwz r9, 0x14(r1)
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCA88:
    mr r6, r27
    addi r3, r23, 0x50
    li r4, 0x6
    li r5, 0x3
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r24, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r6, r30
    mr r9, r24
    li r4, 0x1
    lwz r5, 0x0(r25)
    li r8, 0x6
    lwz r7, 0x14(r1)
    li r10, 0x7
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCAEC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r8, r30
    li r4, 0x7
    li r5, 0x0
    lwz r7, 0x0(r25)
    li r6, 0x1
    lwz r9, 0x14(r1)
    li r10, 0x7
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCB34:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x14(r1)
    mr r3, r31
    mr r10, r30
    li r4, 0x7
    stw r0, 0x8(r1)
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCB7C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x0(r25)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    stw r0, 0x8(r1)
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCBC4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x0(r25)
    mr r3, r31
    mr r4, r30
    li r6, 0x7
    stw r0, 0x8(r1)
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    lwz r5, 0x14(r1)
    li r10, 0x1
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCC0C:
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x0(r25)
    mr r3, r31
    mr r4, r30
    li r6, 0x7
    stw r0, 0x8(r1)
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    lwz r5, 0x14(r1)
    li r10, 0x1
    bl fn_801B64EC
    b @L_801BCC64
@L_801BCC54:
    addi r3, r2, -22512
    li r4, 0x5e4
    addi r5, r2, -22480
    bl fn_80196E10
@L_801BCC64:
    cmpwi r28, 0x0
    stw r31, 0x0(r25)
    bne @L_801BCF1C
    lwz r3, 0x4c(r23)
    lis r0, 0x40
    rlwinm r3, r3, 0, 8, 11
    cmpw r3, r0
    beq @L_801BCDE8
    bge @L_801BCCC4
    lis r0, 0x20
    cmpw r3, r0
    beq @L_801BCD3C
    bge @L_801BCCB4
    lis r0, 0x10
    cmpw r3, r0
    beq @L_801BCCF4
    bge @L_801BCF08
    cmpwi r3, 0x0
    beq @L_801BCE30
    b @L_801BCF08
@L_801BCCB4:
    lis r0, 0x30
    cmpw r3, r0
    beq @L_801BCDA0
    b @L_801BCF08
@L_801BCCC4:
    lis r0, 0x60
    cmpw r3, r0
    beq @L_801BCE78
    bge @L_801BCCE4
    lis r0, 0x50
    cmpw r3, r0
    beq @L_801BCE30
    b @L_801BCF08
@L_801BCCE4:
    lis r0, 0x70
    cmpw r3, r0
    beq @L_801BCEC0
    b @L_801BCF08
@L_801BCCF4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r6, r29
    mr r8, r29
    li r4, 0x5
    lwz r5, 0x0(r26)
    li r10, 0x7
    lwz r7, 0x10(r1)
    lwz r9, 0x10(r1)
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCD3C:
    mr r6, r27
    addi r3, r23, 0x50
    li r4, 0x6
    li r5, 0x3
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r24, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r6, r29
    mr r9, r24
    li r4, 0x5
    lwz r5, 0x0(r26)
    li r8, 0x6
    lwz r7, 0x10(r1)
    li r10, 0x7
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCDA0:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r8, r29
    li r4, 0x7
    li r5, 0x0
    lwz r7, 0x0(r26)
    li r6, 0x5
    lwz r9, 0x10(r1)
    li r10, 0x7
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCDE8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    lwz r0, 0x10(r1)
    mr r3, r31
    mr r10, r29
    li r4, 0x7
    stw r0, 0x8(r1)
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCE30:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    lwz r0, 0x0(r26)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    stw r0, 0x8(r1)
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x5
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCE78:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    lwz r0, 0x0(r26)
    mr r3, r31
    mr r4, r29
    li r6, 0x7
    stw r0, 0x8(r1)
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    lwz r5, 0x10(r1)
    li r10, 0x5
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCEC0:
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    lwz r0, 0x0(r26)
    mr r3, r31
    mr r4, r29
    li r6, 0x7
    stw r0, 0x8(r1)
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    lwz r5, 0x10(r1)
    li r10, 0x5
    bl fn_801B5F08
    b @L_801BCF18
@L_801BCF08:
    addi r3, r2, -22512
    li r4, 0x61e
    addi r5, r2, -22480
    bl fn_80196E10
@L_801BCF18:
    stw r31, 0x0(r26)
@L_801BCF1C:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

