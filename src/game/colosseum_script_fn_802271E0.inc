    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r24, r3
    mr r27, r4
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r23, r3
    bl fn_80205184
    mr r0, r3
    mr r3, r23
    mr r28, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r26, r3
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mr r0, r3
    li r3, 0x12
    mr r25, r0
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_802040E8
    mr r30, r3
    mr r3, r29
    bl fn_80203FE4
    mr r23, r3
    mr r3, r29
    bl fn_80203EDC
    mr r31, r3
    mr r3, r29
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r27, 0, 24, 31
    rlwinm r27, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_802272F4
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r3, 28, 0, 3
    rlwinm r3, r4, 1, 31, 31
    cmpwi r25, 0x0
    subf r0, r3, r0
    rlwinm r0, r0, 4, 0, 31
    add r0, r0, r3
    subfic r0, r0, 0x64
    rlwinm r0, r0, 0, 16, 31
    beq @L_802272F4
    mullw r25, r25, r0
    li r0, 0x64
    divw. r25, r25, r0
    bne @L_802272DC
    li r25, 0x1
@L_802272DC:
    mr r3, r26
    mr r7, r25
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_802272F4:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x27
    bne @L_80227338
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r0, r31
    bge @L_80227338
    mr r3, r29
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_80227338:
    mr r3, r29
    li r4, 0x14
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022747C
    mr r4, r28
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x65
    bne @L_80227378
    rlwinm r0, r24, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802273A8
@L_80227378:
    mr r3, r29
    li r4, 0x2c
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802273A8
    mr r3, r29
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8022747C
@L_802273A8:
    mr r3, r29
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    cmpw r3, r25
    bgt @L_8022747C
    mr r3, r26
    subi r7, r27, 0x1
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r29
    li r4, 0x2c
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80227420
    mr r3, r26
    li r4, 0x46
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022747C
    mr r3, r26
    li r4, 0x46
    li r5, 0x0
    bl fn_802097C8
    b @L_8022747C
@L_80227420:
    mr r3, r29
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8022747C
    mr r3, r26
    li r4, 0x47
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80227464
    mr r3, r26
    li r4, 0x47
    li r5, 0x0
    bl fn_802097C8
@L_80227464:
    rlwinm r7, r30, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
@L_8022747C:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
