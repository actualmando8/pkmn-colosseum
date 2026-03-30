    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    extsb. r0, r9
    stmw r22, 0x8(r1)
    mr r23, r3
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r27, r7
    mr r28, r8
    li r30, 0x0
    bge @L_8022B318
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 24, 31
    b @L_8022B31C
@L_8022B318:
    rlwinm r3, r9, 0, 24, 31
@L_8022B31C:
    rlwinm r0, r24, 0, 16, 31
    mr r31, r3
    cmplwi r0, 0xae
    bne @L_8022B344
    mr r3, r23
    li r4, 0x7
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022B344
    li r31, 0x5
@L_8022B344:
    mr r4, r24
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r29, r3, 0, 16, 31
    mr r5, r23
    li r3, 0x0
    li r4, 0x0
    li r6, 0x1
    bl fn_801F2654
    mr r22, r3
    mr r6, r23
    li r3, 0x0
    li r4, 0x1f
    li r5, 0x2
    bl fn_801F3624
    rlwinm r0, r31, 0, 24, 31
    mr r31, r3
    cmplwi r0, 0x7
    bgt @L_8022B5B0
    lis r3, jumptable_8039A2F4@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A2F4@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8022B510
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B40C
    cmplwi r26, 0x0
    beq @L_8022B3F0
    mr r12, r26
    mr r3, r23
    mr r4, r24
    mr r5, r25
    mtctr r12
    bctrl
    mr r30, r3
    b @L_8022B41C
@L_8022B3F0:
    mr r6, r23
    li r3, 0x0
    li r4, 0x1
    li r5, 0x2
    bl fn_801F2598
    mr r30, r3
    b @L_8022B41C
@L_8022B40C:
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
@L_8022B41C:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B5B0
    mr r3, r30
    bl fn_80207BF4
    mr r24, r3
    mr r4, r23
    li r3, 0x3
    bl fn_801F025C
    mr r26, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r22, r3, 0, 16, 31
    mr r3, r26
    li r25, 0x0
    li r4, 0x4d
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B4B0
    mr r3, r26
    li r4, 0x4d
    bl fn_801F6C4C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8022B4B0
    mr r4, r22
    bl fn_801F00D0
    mr. r22, r3
    beq @L_8022B4B0
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B4B0
    mr r25, r22
@L_8022B4B0:
    cmplwi r25, 0x0
    beq @L_8022B4C0
    mr r30, r25
    b @L_8022B5B0
@L_8022B4C0:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x1f
    beq @L_8022B5B0
    cmplwi r29, 0xd
    bne @L_8022B5B0
    rlwinm. r0, r31, 0, 16, 31
    beq @L_8022B5B0
    mr r7, r23
    li r3, 0x0
    li r4, 0x1f
    li r5, 0x1
    li r6, 0x2
    bl fn_801F349C
    li r4, 0x0
    mr r30, r3
    li r5, 0x114
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8022B5B0
@L_8022B510:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B5B0
    mr r6, r23
    li r3, 0x0
    li r4, 0x1
    li r5, 0x3
    bl fn_801F2598
    mr r30, r3
    b @L_8022B5B0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B5B0
    mr r4, r23
    mr r5, r25
    li r3, 0xf
    bl fn_801F02AC
    mr r30, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022B5B0
    mr r4, r30
    li r3, 0xe
    bl fn_801F025C
    mr r30, r3
    b @L_8022B5B0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B5B0
    mr r6, r23
    li r3, 0x0
    li r4, 0x1
    li r5, 0x2
    bl fn_801F2598
    mr r30, r3
    b @L_8022B5B0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B5B0
    mr r30, r23
@L_8022B5B0:
    mr r3, r30
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
