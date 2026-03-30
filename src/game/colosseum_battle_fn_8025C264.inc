    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r29, r7
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r26
    bl fn_801F0134
    mr r31, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    mr r4, r27
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r28, 0x0
    clrlwi r30, r3, 16
    bne @L_8025C2E4
    li r3, 0x1
    b @L_8025C590
@L_8025C2E4:
    addi r7, r1, 0x8
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    bl fn_801F1D5C
    mr r24, r3
    addi r4, r1, 0x8
    li r3, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_801F3BB4
    addi r4, r1, 0x8
    clrlwi r0, r24, 16
    li r6, 0x0
    li r7, 0x0
    li r5, 0x0
    b @L_8025C358
@L_8025C32C:
    clrlslwi r3, r5, 16, 2
    lwzx r3, r4, r3
    cmplwi r3, 0x0
    beq @L_8025C354
    cmplw r26, r3
    bne @L_8025C348
    mr r6, r5
@L_8025C348:
    cmplw r28, r3
    bne @L_8025C354
    mr r7, r5
@L_8025C354:
    addi r5, r5, 0x1
@L_8025C358:
    clrlwi r3, r5, 16
    cmplw r3, r0
    blt @L_8025C32C
    cmplwi r29, 0xffff
    clrlwi r3, r6, 16
    clrlwi r0, r7, 16
    subf r0, r0, r3
    srwi r24, r0, 31
    beq @L_8025C384
    cmplwi r29, 0xfffe
    bne @L_8025C420
@L_8025C384:
    cmplwi r29, 0xffff
    bne @L_8025C3CC
    mr r3, r25
    mr r4, r28
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C3CC
    mr r3, r28
    li r4, 0x1d
    bl fn_80201D84
    clrlwi r3, r3, 16
    clrlwi r0, r31, 16
    cmplw r0, r3
    bne @L_8025C3CC
    li r3, -0x1
    b @L_8025C590
@L_8025C3CC:
    mr r3, r25
    mr r4, r26
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C3F4
    cmplwi r24, 0x1
    bne @L_8025C3F4
    li r3, 0x0
    b @L_8025C590
@L_8025C3F4:
    mr r3, r25
    mr r4, r28
    mr r5, r27
    bl fn_8025CAA8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C418
    li r3, 0x0
    b @L_8025C590
@L_8025C418:
    li r3, 0x1
    b @L_8025C590
@L_8025C420:
    mr r3, r27
    mr r4, r26
    mr r5, r28
    bl fn_80229934
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C444
    li r3, 0x0
    b @L_8025C590
@L_8025C444:
    mr r3, r25
    mr r4, r28
    mr r5, r27
    bl fn_8025CAA8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C468
    li r3, 0x0
    b @L_8025C590
@L_8025C468:
    mr r3, r25
    mr r4, r28
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C4A8
    mr r3, r28
    li r4, 0x1d
    bl fn_80201D84
    clrlwi r3, r3, 16
    clrlwi r0, r31, 16
    cmplw r3, r0
    bne @L_8025C4A8
    li r3, -0x1
    b @L_8025C590
@L_8025C4A8:
    cmplwi r24, 0x1
    bne @L_8025C558
    mr r3, r25
    mr r4, r28
    li r5, 0x1f
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C4F4
    cmplwi r30, 0x92
    beq @L_8025C4F4
    cmplwi r30, 0x95
    beq @L_8025C4F4
    cmplwi r30, 0x98
    beq @L_8025C4F4
    cmplwi r30, 0xcf
    beq @L_8025C4F4
    li r3, 0x0
    b @L_8025C590
@L_8025C4F4:
    mr r3, r25
    mr r4, r28
    li r5, 0x20
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C520
    cmplwi r30, 0x93
    beq @L_8025C520
    li r3, 0x0
    b @L_8025C590
@L_8025C520:
    mr r3, r25
    mr r4, r28
    li r5, 0x21
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C558
    clrlwi r0, r27, 16
    cmplwi r0, 0x39
    beq @L_8025C558
    cmplwi r0, 0xfa
    beq @L_8025C558
    li r3, 0x0
    b @L_8025C590
@L_8025C558:
    mr r3, r27
    bl fn_80229B70
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C574
    li r3, -0x1
    b @L_8025C590
@L_8025C574:
    mr r3, r27
    bl fn_80229BD8
    clrlwi r0, r3, 24
    li r3, 0x1
    cmplwi r0, 0x1
    bne @L_8025C590
    li r3, -0x1
@L_8025C590:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
