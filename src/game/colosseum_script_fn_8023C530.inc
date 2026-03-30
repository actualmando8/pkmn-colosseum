    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r22, 0x28(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r6
    mr r28, r3
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C5C0
    mr r4, r30
    li r3, 0x0
    li r5, 0x1d
    li r6, 0x0
    bl fn_8011BEB4
    mr. r25, r3
    bne @L_8023C5C8
    lis r3, -32730
    addi r0, r3, -15780
    mr r25, r0
    b @L_8023C5C8
@L_8023C5C0:
    li r3, 0x1
    b @L_8023CA88
@L_8023C5C8:
    mr r4, r28
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r22, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    li r3, 0x0
    cmplwi r0, 0x7
    bgt @L_8023CA88
    lis r4, jumptable_8039A5B8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, jumptable_8039A5B8@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    mr r3, r28
    mr r4, r30
    bl fn_80239500
    mr r23, r3
    mr r3, r28
    mr r4, r30
    mr r5, r29
    bl fn_802395C8
    mr r22, r3
    mr r3, r31
    li r27, 0x1
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023C660
    li r27, 0x0
    b @L_8023C758
@L_8023C660:
    rlwinm. r0, r30, 0, 16, 31
    beq @L_8023C758
    cmplwi r0, 0x165
    beq @L_8023C758
    mr r3, r28
    mr r4, r31
    li r5, 0xa
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C6A4
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0xd
    bne @L_8023C6A4
    extsh. r0, r23
    beq @L_8023C6A4
    li r27, 0x0
@L_8023C6A4:
    mr r3, r28
    mr r4, r31
    li r5, 0xb
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C6D8
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0xb
    bne @L_8023C6D8
    extsh. r0, r23
    beq @L_8023C6D8
    li r27, 0x0
@L_8023C6D8:
    mr r3, r28
    mr r4, r31
    li r5, 0x12
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C718
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8023C718
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023C718
    li r27, 0x0
@L_8023C718:
    mr r3, r28
    mr r4, r31
    li r5, 0x2b
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C758
    mr r4, r30
    li r3, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C758
    li r27, 0x0
@L_8023C758:
    rlwinm. r0, r27, 0, 24, 31
    bne @L_8023C768
    li r3, 0x0
    b @L_8023CA88
@L_8023C768:
    mr r12, r25
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
    b @L_8023CA88
    rlwinm r23, r22, 0, 16, 31
    li r24, 0x0
    b @L_8023C914
@L_8023C794:
    mr r3, r28
    mr r4, r30
    bl fn_80239500
    mr r22, r3
    mr r3, r28
    mr r4, r30
    mr r5, r29
    bl fn_802395C8
    mr r27, r3
    mr r3, r31
    li r26, 0x1
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023C7D4
    li r26, 0x0
    b @L_8023C8CC
@L_8023C7D4:
    rlwinm. r0, r30, 0, 16, 31
    beq @L_8023C8CC
    cmplwi r0, 0x165
    beq @L_8023C8CC
    mr r3, r28
    mr r4, r31
    li r5, 0xa
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C818
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xd
    bne @L_8023C818
    extsh. r0, r22
    beq @L_8023C818
    li r26, 0x0
@L_8023C818:
    mr r3, r28
    mr r4, r31
    li r5, 0xb
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C84C
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xb
    bne @L_8023C84C
    extsh. r0, r22
    beq @L_8023C84C
    li r26, 0x0
@L_8023C84C:
    mr r3, r28
    mr r4, r31
    li r5, 0x12
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C88C
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8023C88C
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023C88C
    li r26, 0x0
@L_8023C88C:
    mr r3, r28
    mr r4, r31
    li r5, 0x2b
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C8CC
    mr r4, r30
    li r3, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C8CC
    li r26, 0x0
@L_8023C8CC:
    rlwinm. r0, r26, 0, 24, 31
    bne @L_8023C8DC
    li r3, 0x0
    b @L_8023C900
@L_8023C8DC:
    mr r12, r25
    clrlslwi r0, r24, 16, 2
    addi r6, r1, 0x8
    mr r3, r28
    mr r4, r29
    mr r5, r30
    lwzx r6, r6, r0
    mtctr r12
    bctrl
@L_8023C900:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C910
    b @L_8023CA88
@L_8023C910:
    addi r24, r24, 0x1
@L_8023C914:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r23
    blt @L_8023C794
    b @L_8023CA88
    mr r3, r28
    mr r4, r30
    bl fn_80239500
    mr r27, r3
    mr r3, r28
    mr r4, r30
    mr r5, r29
    bl fn_802395C8
    mr r24, r3
    mr r3, r31
    li r26, 0x1
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023C964
    li r26, 0x0
    b @L_8023CA5C
@L_8023C964:
    rlwinm. r0, r30, 0, 16, 31
    beq @L_8023CA5C
    cmplwi r0, 0x165
    beq @L_8023CA5C
    mr r3, r28
    mr r4, r31
    li r5, 0xa
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C9A8
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0xd
    bne @L_8023C9A8
    extsh. r0, r27
    beq @L_8023C9A8
    li r26, 0x0
@L_8023C9A8:
    mr r3, r28
    mr r4, r31
    li r5, 0xb
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C9DC
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0xb
    bne @L_8023C9DC
    extsh. r0, r27
    beq @L_8023C9DC
    li r26, 0x0
@L_8023C9DC:
    mr r3, r28
    mr r4, r31
    li r5, 0x12
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023CA1C
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8023CA1C
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023CA1C
    li r26, 0x0
@L_8023CA1C:
    mr r3, r28
    mr r4, r31
    li r5, 0x2b
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023CA5C
    mr r4, r30
    li r3, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023CA5C
    li r26, 0x0
@L_8023CA5C:
    rlwinm. r0, r26, 0, 24, 31
    bne @L_8023CA6C
    li r3, 0x0
    b @L_8023CA88
@L_8023CA6C:
    mr r12, r25
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
@L_8023CA88:
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
