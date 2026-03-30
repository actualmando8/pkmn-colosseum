    nofralloc
    stwu r1, -0x320(r1)
    mflr r0
    stw r0, 0x324(r1)
    stmw r14, 0x2d8(r1)
    mr r21, r4
    mr r16, r5
    mr r17, r6
    mr r15, r3
    mr r22, r7
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r14, r3, 0, 16, 31
    mr r3, r15
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801FB1C0
    mr. r19, r3
    beq @L_80233D9C
    mr r3, r15
    li r4, 0x0
    li r5, 0x45
    li r6, 0x1
    bl fn_801FB1C0
    mr. r18, r3
    beq @L_80233D9C
    li r0, 0x2a
    addi r6, r1, 0x178
    subi r4, r19, 0x4
    mtctr r0
@L_80233288:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_80233288
    lwz r3, 0x4(r4)
    li r0, 0x2a
    addi r5, r1, 0x24
    subi r4, r18, 0x4
    stw r3, 0x4(r6)
    mtctr r0
@L_802332B4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_802332B4
    lwz r3, 0x4(r4)
    rlwinm r0, r17, 0, 16, 31
    li r6, 0x0
    li r4, 0x0
    stw r3, 0x4(r5)
    b @L_802332EC
@L_802332E0:
    clrlslwi r3, r6, 16, 2
    addi r6, r6, 0x1
    stwx r4, r16, r3
@L_802332EC:
    rlwinm r3, r6, 0, 16, 31
    cmplw r3, r0
    blt @L_802332E0
    li r5, 0x0
    addi r3, r1, 0x8
    mr r4, r5
    li r20, 0x0
    b @L_80233318
@L_8023330C:
    rlwinm r0, r5, 0, 16, 31
    addi r5, r5, 0x1
    stbx r4, r3, r0
@L_80233318:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8023330C
    mr r4, r14
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    li r23, 0x1
    cmplwi r0, 0x1
    beq @L_8023334C
    mr r23, r22
@L_8023334C:
    mr r4, r14
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233498
    addi r26, r1, 0x10
    addi r24, r1, 0x8
    rlwinm r22, r17, 0, 16, 31
@L_80233378:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r22
    bge @L_8023380C
    mr r3, r15
    mr r4, r21
    mr r5, r16
    mr r6, r17
    addi r8, r1, 0x10
    li r7, 0x0
    bl fn_80233DB0
    rlwinm. r0, r3, 0, 16, 31
    mr r23, r3
    beq @L_8023380C
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r23, 0, 16, 31
    divw r0, r5, r4
    clrlslwi r25, r20, 16, 2
    rlwinm r23, r20, 0, 16, 31
    li r3, 0x0
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r0, r26, r0
    stwx r0, r16, r25
    bl fn_801F4804
    lwzx r4, r16, r25
    mr r5, r3
    mr r3, r19
    bl fn_80206AEC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 16, 31
    mr r3, r19
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    mr r4, r27
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233488
    mr r4, r25
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_8023348C
@L_80233488:
    li r0, 0x0
@L_8023348C:
    stbx r0, r24, r23
    addi r20, r20, 0x1
    b @L_80233378
@L_80233498:
    addi r26, r1, 0x10
    addi r25, r1, 0x8
    rlwinm r22, r23, 0, 16, 31
    li r24, 0x0
@L_802334A8:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r22
    bge @L_8023351C
    mr r3, r15
    mr r4, r21
    mr r5, r16
    mr r6, r17
    addi r8, r1, 0x10
    li r7, 0x1
    bl fn_80233DB0
    rlwinm. r0, r3, 0, 16, 31
    mr r27, r3
    beq @L_8023351C
    bl fn_800E0C54
    rlwinm r6, r3, 0, 16, 31
    rlwinm r5, r27, 0, 16, 31
    divw r4, r6, r5
    rlwinm r7, r20, 0, 16, 31
    li r0, 0x2
    stbx r0, r25, r7
    clrlslwi r3, r20, 16, 2
    addi r20, r20, 0x1
    mullw r0, r4, r5
    addi r24, r24, 0x1
    subf r0, r0, r6
    clrlslwi r0, r0, 16, 2
    lwzx r0, r26, r0
    stwx r0, r16, r3
    b @L_802334A8
@L_8023351C:
    addi r25, r1, 0x10
    addi r24, r1, 0x8
    rlwinm r22, r23, 0, 16, 31
    li r23, 0x0
@L_8023352C:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r22
    bge @L_802335A0
    mr r3, r15
    mr r4, r21
    mr r5, r16
    mr r6, r17
    addi r8, r1, 0x10
    li r7, 0x2
    bl fn_80233DB0
    rlwinm. r0, r3, 0, 16, 31
    mr r26, r3
    beq @L_802335A0
    bl fn_800E0C54
    rlwinm r6, r3, 0, 16, 31
    rlwinm r5, r26, 0, 16, 31
    divw r4, r6, r5
    rlwinm r7, r20, 0, 16, 31
    li r0, 0x3
    stbx r0, r24, r7
    clrlslwi r3, r20, 16, 2
    addi r20, r20, 0x1
    mullw r0, r4, r5
    addi r23, r23, 0x1
    subf r0, r0, r6
    clrlslwi r0, r0, 16, 2
    lwzx r0, r25, r0
    stwx r0, r16, r3
    b @L_8023352C
@L_802335A0:
    addi r26, r1, 0x10
    addi r24, r1, 0x8
    rlwinm r22, r17, 0, 16, 31
@L_802335AC:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r22
    bge @L_802336CC
    mr r3, r15
    mr r4, r21
    mr r5, r16
    mr r6, r17
    addi r8, r1, 0x10
    li r7, 0x3
    bl fn_80233DB0
    rlwinm. r0, r3, 0, 16, 31
    mr r23, r3
    beq @L_802336CC
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r23, 0, 16, 31
    divw r0, r5, r4
    clrlslwi r25, r20, 16, 2
    rlwinm r23, r20, 0, 16, 31
    li r3, 0x0
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r0, r26, r0
    stwx r0, r16, r25
    bl fn_801F4804
    lwzx r4, r16, r25
    mr r5, r3
    mr r3, r19
    bl fn_80206AEC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 16, 31
    mr r3, r19
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    mr r4, r27
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802336BC
    mr r4, r25
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_802336C0
@L_802336BC:
    li r0, 0x0
@L_802336C0:
    stbx r0, r24, r23
    addi r20, r20, 0x1
    b @L_802335AC
@L_802336CC:
    addi r22, r1, 0x10
    addi r25, r1, 0x8
    rlwinm r23, r17, 0, 16, 31
@L_802336D8:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r23
    bge @L_8023380C
    mr r3, r15
    mr r4, r21
    mr r5, r16
    mr r6, r17
    addi r8, r1, 0x10
    li r7, 0x0
    bl fn_80233DB0
    rlwinm. r0, r3, 0, 16, 31
    mr r24, r3
    beq @L_8023380C
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r24, 0, 16, 31
    divw r0, r5, r4
    clrlslwi r26, r20, 16, 2
    rlwinm r24, r20, 0, 16, 31
    li r3, 0x0
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r0, r22, r0
    stwx r0, r16, r26
    bl fn_801F4804
    lwzx r4, r16, r26
    mr r5, r3
    mr r3, r19
    bl fn_80206AEC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 16, 31
    mr r3, r19
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r26, r3, 0, 16, 31
    mr r4, r27
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802337E8
    mr r4, r26
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_802337EC
@L_802337E8:
    li r0, 0x0
@L_802337EC:
    stbx r0, r25, r24
    lbzx r0, r25, r24
    cmplwi r0, 0x2
    bne @L_80233804
    li r0, 0x3
    stbx r0, r25, r24
@L_80233804:
    addi r20, r20, 0x1
    b @L_802336D8
@L_8023380C:
    mr r4, r14
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80233D44
    mr r4, r14
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233914
    rlwinm r21, r17, 0, 16, 31
    li r15, 0x0
    b @L_80233904
@L_80233858:
    clrlslwi r24, r15, 16, 2
    rlwinm r14, r15, 0, 16, 31
    lwzx r0, r16, r24
    cmplwi r0, 0x0
    beq @L_80233900
    addi r0, r15, 0x1
    addi r25, r1, 0x8
    rlwinm r20, r0, 0, 16, 31
    rlwinm r22, r17, 0, 16, 31
    b @L_802338F4
@L_80233880:
    clrlslwi r23, r20, 16, 2
    rlwinm r26, r20, 0, 16, 31
    lwzx r0, r16, r23
    cmplwi r0, 0x0
    beq @L_802338F0
    lwzx r3, r16, r24
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    lwzx r3, r16, r23
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmplw r27, r0
    ble @L_802338F0
    addi r4, r1, 0x8
    lwzx r5, r16, r24
    lwzx r3, r16, r23
    lbzx r6, r25, r14
    lbzx r0, r4, r26
    stwx r3, r16, r24
    stbx r0, r25, r14
    stwx r5, r16, r23
    stbx r6, r4, r26
@L_802338F0:
    addi r20, r20, 0x1
@L_802338F4:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r22
    blt @L_80233880
@L_80233900:
    addi r15, r15, 0x1
@L_80233904:
    rlwinm r0, r15, 0, 16, 31
    cmplw r0, r21
    blt @L_80233858
    b @L_80233D44
@L_80233914:
    rlwinm r0, r17, 0, 16, 31
    li r12, 0x0
    b @L_8023399C
@L_80233920:
    clrlslwi r7, r12, 16, 2
    rlwinm r9, r12, 0, 16, 31
    lwzx r3, r16, r7
    cmplwi r3, 0x0
    beq @L_80233998
    addi r3, r12, 0x1
    addi r8, r1, 0x8
    rlwinm r14, r3, 0, 16, 31
    rlwinm r3, r17, 0, 16, 31
    b @L_8023398C
@L_80233948:
    clrlslwi r6, r14, 16, 2
    rlwinm r10, r14, 0, 16, 31
    lwzx r11, r16, r6
    cmplwi r11, 0x0
    beq @L_80233988
    lbzx r5, r8, r9
    cmplwi r5, 0x2
    beq @L_80233988
    lbzx r4, r8, r10
    cmplwi r4, 0x2
    bne @L_80233988
    lwzx r20, r16, r7
    stbx r4, r8, r9
    stwx r11, r16, r7
    stwx r20, r16, r6
    stbx r5, r8, r10
@L_80233988:
    addi r14, r14, 0x1
@L_8023398C:
    rlwinm r4, r14, 0, 16, 31
    cmplw r4, r3
    blt @L_80233948
@L_80233998:
    addi r12, r12, 0x1
@L_8023399C:
    rlwinm r3, r12, 0, 16, 31
    cmplw r3, r0
    blt @L_80233920
    rlwinm r0, r17, 0, 16, 31
    li r12, 0x0
    b @L_80233A58
@L_802339B4:
    clrlslwi r6, r12, 16, 2
    rlwinm r7, r12, 0, 16, 31
    lwzx r3, r16, r6
    cmplwi r3, 0x0
    beq @L_80233A54
    addi r8, r1, 0x8
    lbzx r3, r8, r7
    cmplwi r3, 0x2
    beq @L_80233A54
    addi r4, r12, 0x1
    rlwinm r3, r17, 0, 16, 31
    rlwinm r14, r4, 0, 16, 31
    b @L_80233A48
@L_802339E8:
    clrlslwi r5, r14, 16, 2
    rlwinm r9, r14, 0, 16, 31
    lwzx r10, r16, r5
    cmplwi r10, 0x0
    beq @L_80233A44
    lbzx r4, r8, r7
    cmplwi r4, 0x0
    beq @L_80233A44
    cmplwi r4, 0x1
    beq @L_80233A44
    addi r11, r1, 0x8
    lbzx r4, r11, r9
    cmplwi r4, 0x0
    beq @L_80233A28
    cmplwi r4, 0x1
    bne @L_80233A44
@L_80233A28:
    lwzx r20, r16, r6
    lbzx r21, r8, r7
    lbzx r4, r11, r9
    stwx r10, r16, r6
    stbx r4, r8, r7
    stwx r20, r16, r5
    stbx r21, r11, r9
@L_80233A44:
    addi r14, r14, 0x1
@L_80233A48:
    rlwinm r4, r14, 0, 16, 31
    cmplw r4, r3
    blt @L_802339E8
@L_80233A54:
    addi r12, r12, 0x1
@L_80233A58:
    rlwinm r3, r12, 0, 16, 31
    cmplw r3, r0
    blt @L_802339B4
    rlwinm r0, r17, 0, 16, 31
    li r21, 0x0
    stw r0, 0x2d4(r1)
    b @L_80233D34
@L_80233A74:
    clrlslwi r30, r21, 16, 2
    rlwinm r24, r21, 0, 16, 31
    lwzx r0, r16, r30
    cmplwi r0, 0x0
    beq @L_80233D30
    addi r3, r1, 0x8
    lbzx r0, r3, r24
    cmplwi r0, 0x1
    beq @L_80233AA0
    cmplwi r0, 0x0
    bne @L_80233D30
@L_80233AA0:
    rlwinm r0, r17, 0, 16, 31
    addi r3, r21, 0x1
    stw r0, 0x2d0(r1)
    rlwinm r20, r3, 0, 16, 31
    b @L_80233D20
@L_80233AB4:
    clrlslwi r31, r20, 16, 2
    rlwinm r23, r20, 0, 16, 31
    lwzx r0, r16, r31
    cmplwi r0, 0x0
    beq @L_80233D1C
    addi r22, r1, 0x8
    lbzx r0, r22, r23
    cmplwi r0, 0x1
    beq @L_80233AE0
    cmplwi r0, 0x0
    bne @L_80233D1C
@L_80233AE0:
    li r29, 0x0
    mr r14, r29
    b @L_80233BDC
@L_80233AEC:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r27, r3, 0, 16, 31
    mr r3, r19
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    mr r4, r27
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233BBC
    mr r3, r15
    mr r4, r19
    mr r5, r26
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80233BA0
    mr r4, r25
    rlwinm r6, r14, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_80233BC0
@L_80233BA0:
    mr r4, r25
    rlwinm r6, r14, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_80233BC0
@L_80233BBC:
    li r3, 0x9
@L_80233BC0:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_80233BD8
    li r4, 0x0
    bl fn_8010C54C
    add r29, r29, r3
@L_80233BD8:
    addi r14, r14, 0x1
@L_80233BDC:
    rlwinm r0, r14, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80233AEC
    li r28, 0x0
    mr r27, r28
    b @L_80233CE4
@L_80233BF4:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r25, r3, 0, 16, 31
    mr r3, r15
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r14, r3, 0, 16, 31
    mr r3, r18
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r26, r3, 0, 16, 31
    mr r4, r14
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233CC4
    mr r3, r15
    mr r4, r18
    mr r5, r25
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80233CA8
    mr r4, r26
    rlwinm r6, r27, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_80233CC8
@L_80233CA8:
    mr r4, r26
    rlwinm r6, r27, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    b @L_80233CC8
@L_80233CC4:
    li r3, 0x9
@L_80233CC8:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_80233CE0
    li r4, 0x0
    bl fn_8010C54C
    add r28, r28, r3
@L_80233CE0:
    addi r27, r27, 0x1
@L_80233CE4:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80233BF4
    cmpw r29, r28
    bge @L_80233D1C
    addi r3, r1, 0x8
    lwzx r5, r16, r30
    lwzx r4, r16, r31
    lbzx r6, r3, r24
    lbzx r0, r22, r23
    stwx r4, r16, r30
    stbx r0, r3, r24
    stwx r5, r16, r31
    stbx r6, r22, r23
@L_80233D1C:
    addi r20, r20, 0x1
@L_80233D20:
    lwz r0, 0x2d0(r1)
    rlwinm r3, r20, 0, 16, 31
    cmplw r3, r0
    blt @L_80233AB4
@L_80233D30:
    addi r21, r21, 0x1
@L_80233D34:
    lwz r0, 0x2d4(r1)
    rlwinm r3, r21, 0, 16, 31
    cmplw r3, r0
    blt @L_80233A74
@L_80233D44:
    li r0, 0x2a
    subi r6, r19, 0x4
    addi r4, r1, 0x178
    mtctr r0
@L_80233D54:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_80233D54
    lwz r3, 0x4(r4)
    li r0, 0x2a
    subi r5, r18, 0x4
    addi r4, r1, 0x24
    stw r3, 0x4(r6)
    mtctr r0
@L_80233D80:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80233D80
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
@L_80233D9C:
    lmw r14, 0x2d8(r1)
    lwz r0, 0x324(r1)
    mtlr r0
    addi r1, r1, 0x320
    blr
