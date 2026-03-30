    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r23, 0x4c(r1)
    mr r26, r3
    mr r24, r4
    mr r23, r5
    mr r25, r6
    li r3, -0x1
    bl fn_800E0C04
    mr r4, r3
    mr r3, r24
    li r5, 0x0
    bl fn_801DE190
    cmplwi r3, 0x0
    stw r3, 0x0(r26)
    bne @L_8025DF84
    li r3, 0x0
    b @L_8025E09C
@L_8025DF84:
    li r24, 0x0
    b @L_8025DFB8
@L_8025DF8C:
    lwz r3, 0x0(r26)
    li r6, 0x0
    lhz r4, 0x0(r23)
    lhz r5, 0x2(r23)
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8025DFB0
    li r3, 0x0
    b @L_8025E09C
@L_8025DFB0:
    addi r23, r23, 0x4
    addi r24, r24, 0x1
@L_8025DFB8:
    cmpw r24, r25
    blt @L_8025DF8C
    cmplwi r26, 0x0
    beq @L_8025E098
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_8025E098
    bl fn_801DAC3C
    mr. r28, r3
    beq @L_8025E098
    li r4, 0x1
    bl fn_800E90C8
    bl fn_80115BD8
    mr. r26, r3
    beq @L_8025E098
    li r3, 0x1
    bl fn_8018F470
    mr r23, r3
    mr r3, r26
    bl fn_80115280
    mr r29, r3
    li r27, 0x0
    cmplwi r29, 0x1
    mr r31, r27
    bne @L_8025E098
    mr r25, r27
    addi r30, r1, 0x8
    b @L_8025E05C
@L_8025E028:
    mr r3, r26
    mr r4, r25
    bl fn_8011538C
    mr r24, r3
    bl fn_800FF56C
    mr r4, r24
    bl fn_80113F6C
    cmplwi r3, 0x0
    beq @L_8025E058
    stwx r3, r30, r31
    addi r27, r27, 0x1
    addi r31, r31, 0x4
@L_8025E058:
    addi r25, r25, 0x1
@L_8025E05C:
    cmplw r25, r29
    blt @L_8025E028
    mr r3, r28
    li r4, 0x1
    bl fn_800E9108
    mr r3, r28
    mr r4, r23
    bl fn_800E8FE8
    mr r3, r28
    mr r4, r27
    addi r5, r1, 0x8
    bl fn_800E900C
    mr r3, r28
    li r4, 0x1
    bl fn_800E3C94
@L_8025E098:
    li r3, 0x1
@L_8025E09C:
    lmw r23, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
