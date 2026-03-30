    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r14, 0x48(r1)
    mr r15, r3
    mr r16, r4
    mr r17, r5
    mr r14, r6
    mr r4, r15
    addi r5, r1, 0x1c
    li r25, 0x0
    li r21, 0x0
    li r20, 0x0
    li r19, 0x0
    li r18, 0x0
    li r3, 0x0
    li r6, 0x1
    bl fn_801F1C18
    mr r26, r3
    mr r3, r15
    mr r4, r16
    bl fn_80236520
    mr r0, r3
    mr r3, r15
    mr r22, r0
    mr r4, r14
    bl fn_80236520
    addi r14, r1, 0x1c
    rlwinm r31, r26, 0, 16, 31
    li r24, 0x0
    b @L_8023F398
@L_8023F2F8:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r14, r0
    cmplw r16, r4
    beq @L_8023F394
    mr r3, r15
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r27, r3, 0, 16, 31
    beq @L_8023F394
    lis r4, -32731
    lis r3, -32731
    lis r5, -32731
    li r23, 0x0
    addi r29, r4, -19340
    addi r30, r3, -16448
    addi r28, r5, -6868
    b @L_8023F388
@L_8023F344:
    clrlslwi r0, r23, 16, 1
    addi r3, r1, 0x8
    lhzx r4, r3, r0
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_8023F36C
    mr r3, r28
@L_8023F36C:
    cmplw r3, r29
    bne @L_8023F378
    li r21, 0x1
@L_8023F378:
    cmplw r3, r30
    bne @L_8023F384
    li r20, 0x1
@L_8023F384:
    addi r23, r23, 0x1
@L_8023F388:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r27
    blt @L_8023F344
@L_8023F394:
    addi r24, r24, 0x1
@L_8023F398:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r31
    blt @L_8023F2F8
    lis r3, -32731
    li r23, 0x0
    addi r14, r3, -19340
    b @L_8023F444
@L_8023F3B4:
    mr r4, r23
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    mr. r24, r3
    bne @L_8023F3D8
    lis r3, -32731
    addi r24, r3, -6868
@L_8023F3D8:
    cmplw r24, r14
    beq @L_8023F3F0
    lis r3, -32731
    addi r0, r3, -16448
    cmplw r24, r0
    bne @L_8023F440
@L_8023F3F0:
    mr r4, r15
    mr r7, r23
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r8, 0x0
    bl fn_801F1990
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F440
    lis r3, -32731
    addi r0, r3, -19340
    cmplw r24, r0
    bne @L_8023F42C
    li r19, 0x1
@L_8023F42C:
    lis r3, -32731
    addi r0, r3, -16448
    cmplw r24, r0
    bne @L_8023F440
    li r18, 0x1
@L_8023F440:
    addi r23, r23, 0x1
@L_8023F444:
    lwz r3, lbl_80478DF8(r13)
    rlwinm r4, r23, 0, 16, 31
    lwz r0, 0x0(r3)
    cmplw r4, r0
    blt @L_8023F3B4
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023F470
    rlwinm r0, r20, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F4B8
@L_8023F470:
    mr r4, r15
    li r3, 0x0
    li r5, 0x1d7
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d7
    bl fn_80239EE8
@L_8023F4B8:
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023F4D0
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F518
@L_8023F4D0:
    mr r3, r25
    mr r4, r15
    li r5, 0x1d8
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d8
    bl fn_80239EE8
@L_8023F518:
    mr r3, r15
    mr r4, r16
    li r5, 0x3
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F598
    mr r3, r15
    mr r4, r22
    li r5, 0x4
    bl fn_8023943C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F598
    mr r3, r25
    mr r4, r15
    li r5, 0x1d9
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d9
    bl fn_80239EE8
@L_8023F598:
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0xb6
    beq @L_8023F5AC
    cmplwi r0, 0xc5
    bne @L_8023F610
@L_8023F5AC:
    mr r3, r16
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8023F610
    mr r3, r25
    mr r4, r15
    li r5, 0x1da
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1da
    bl fn_80239EE8
@L_8023F610:
    mr r3, r15
    mr r4, r16
    li r5, 0x7
    bl fn_80237DBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F6D4
    rlwinm r0, r20, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F680
    mr r3, r25
    mr r4, r15
    li r5, 0x1db
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1db
    bl fn_80239EE8
@L_8023F680:
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F6D4
    mr r3, r25
    mr r4, r15
    li r5, 0x1dc
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1dc
    bl fn_80239EE8
@L_8023F6D4:
    mr r3, r15
    mr r4, r16
    li r5, 0x2
    bl fn_80237DBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023F728
    mr r3, r15
    mr r4, r16
    li r5, 0x1a
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8023F728
    mr r3, r15
    mr r4, r16
    li r5, 0x19
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F7D0
@L_8023F728:
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F77C
    mr r3, r25
    mr r4, r15
    li r5, 0x1dd
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1dd
    bl fn_80239EE8
@L_8023F77C:
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F7D0
    mr r3, r25
    mr r4, r15
    li r5, 0x1de
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1de
    bl fn_80239EE8
@L_8023F7D0:
    addi r14, r1, 0x1c
    rlwinm r19, r26, 0, 16, 31
    li r18, 0x0
    b @L_8023F89C
@L_8023F7E0:
    clrlslwi r0, r18, 16, 2
    lwzx r4, r14, r0
    cmplw r16, r4
    beq @L_8023F898
    mr r3, r15
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r4, r3, 0, 16, 31
    mr r20, r3
    beq @L_8023F898
    addi r3, r1, 0x8
    li r21, 0x0
    b @L_8023F87C
@L_8023F81C:
    clrlslwi r0, r21, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0x10a
    bne @L_8023F878
    mr r3, r25
    mr r4, r15
    li r5, 0x1df
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r15
    mr r8, r17
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1df
    bl fn_80239EE8
    b @L_8023F888
@L_8023F878:
    addi r21, r21, 0x1
@L_8023F87C:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r4
    blt @L_8023F81C
@L_8023F888:
    rlwinm r3, r21, 0, 16, 31
    rlwinm r0, r20, 0, 16, 31
    cmplw r3, r0
    blt @L_8023F8A8
@L_8023F898:
    addi r18, r18, 0x1
@L_8023F89C:
    rlwinm r0, r18, 0, 16, 31
    cmplw r0, r19
    blt @L_8023F7E0
@L_8023F8A8:
    mr r3, r25
    lmw r14, 0x48(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
