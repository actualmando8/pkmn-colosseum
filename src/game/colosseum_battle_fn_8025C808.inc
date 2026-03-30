    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    rlwinm. r0, r9, 0, 25, 25
    stmw r21, 0x14(r1)
    mr r21, r9
    mr r22, r3
    mr r23, r5
    mr r24, r6
    mr r25, r7
    mr r26, r8
    li r28, 0x0
    li r27, 0x0
    beq @L_8025C848
    mr r30, r4
    b @L_8025C84C
@L_8025C848:
    mr r30, r23
@L_8025C84C:
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    clrlwi r31, r21, 24
    mr r21, r3
    andi. r0, r31, 0xbf
    rlwinm. r0, r0, 0, 24, 24
    beq @L_8025C870
    li r28, 0x1
@L_8025C870:
    rlwinm. r0, r31, 0, 26, 26
    beq @L_8025C87C
    li r27, 0x1
@L_8025C87C:
    mr r3, r26
    bl fn_8021C090
    mr r0, r3
    mr r3, r30
    mr r29, r0
    li r4, 0x0
    mr r5, r29
    li r6, 0x0
    bl fn_8012640C
    extsb r26, r3
    mr r3, r25
    bl fn_8021C034
    extsb. r0, r3
    bge @L_8025CA80
    mr r3, r21
    li r4, 0x4c
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C8E8
    clrlwi. r0, r28, 24
    bne @L_8025C8E8
    clrlwi r0, r24, 16
    cmplwi r0, 0xae
    beq @L_8025C8E8
    li r3, 0x0
    b @L_8025CA94
@L_8025C8E8:
    clrlwi r0, r24, 16
    cmplwi r0, 0xae
    beq @L_8025C980
    clrlwi r0, r27, 24
    cmplwi r0, 0x1
    beq @L_8025C980
    mr r3, r22
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C968
    mr r3, r23
    mr r4, r24
    bl fn_80229C28
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C968
    li r0, 0x1
    b @L_8025C96C
@L_8025C968:
    li r0, 0x0
@L_8025C96C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_8025C980
    li r3, 0x0
    b @L_8025CA94
@L_8025C980:
    mr r3, r22
    mr r4, r30
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025C9B8
    mr r3, r22
    mr r4, r30
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C9D4
@L_8025C9B8:
    clrlwi. r0, r28, 24
    bne @L_8025C9D4
    clrlwi r0, r24, 16
    cmplwi r0, 0xae
    beq @L_8025C9D4
    li r3, 0x0
    b @L_8025CA94
@L_8025C9D4:
    mr r3, r22
    mr r4, r30
    li r5, 0x33
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CA0C
    clrlwi. r0, r28, 24
    bne @L_8025CA0C
    clrlwi r0, r29, 16
    cmplwi r0, 0xeb
    bne @L_8025CA0C
    li r3, 0x0
    b @L_8025CA94
@L_8025CA0C:
    mr r3, r22
    mr r4, r30
    li r5, 0x34
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CA44
    clrlwi. r0, r28, 24
    bne @L_8025CA44
    clrlwi r0, r29, 16
    cmplwi r0, 0xe6
    bne @L_8025CA44
    li r3, 0x0
    b @L_8025CA94
@L_8025CA44:
    mr r3, r22
    mr r4, r30
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CA70
    clrlwi. r0, r31, 27
    bne @L_8025CA70
    li r3, 0x0
    b @L_8025CA94
@L_8025CA70:
    extsb. r0, r26
    bgt @L_8025CA90
    li r3, 0x0
    b @L_8025CA94
@L_8025CA80:
    cmpwi r26, 0xc
    blt @L_8025CA90
    li r3, 0x0
    b @L_8025CA94
@L_8025CA90:
    li r3, 0x1
@L_8025CA94:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
