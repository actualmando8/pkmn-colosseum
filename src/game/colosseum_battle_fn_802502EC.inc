    nofralloc
    stwu r1, -0x250(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x254(r1)
    stmw r17, 0x214(r1)
    mr r29, r4
    mr r22, r5
    mr r28, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r28
    addi r4, r1, 0x38
    li r30, 0x0
    li r5, 0x14
    li r6, 0x1
    bl fn_801F7C54
    clrlwi. r0, r3, 16
    mr r31, r3
    bne @L_80250358
    li r3, 0x0
    b @L_8025096C
@L_80250358:
    mr r4, r28
    addi r5, r1, 0x18
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    mr r17, r3
    mr r4, r28
    addi r5, r1, 0x60
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    mr r26, r3
    addi r18, r1, 0x18
    clrlwi r17, r17, 16
    li r20, 0x0
    b @L_802503E0
@L_802503A0:
    clrlslwi r0, r20, 16, 2
    lwzx r19, r18, r0
    cmplwi r19, 0x0
    beq @L_802503DC
    mr r3, r19
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_802503DC
    mr r3, r28
    mr r4, r19
    bl fn_80235714
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802503DC
    li r30, 0x0
@L_802503DC:
    addi r20, r20, 0x1
@L_802503E0:
    clrlwi r0, r20, 16
    cmplw r0, r17
    blt @L_802503A0
    addi r3, r1, 0xc0
    li r5, 0x0
    li r4, 0x0
    b @L_80250408
@L_802503FC:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
@L_80250408:
    clrlwi r0, r5, 16
    cmplwi r0, 0x14
    blt @L_802503FC
    addi r20, r1, 0x38
    clrlwi r27, r31, 16
    li r24, 0x0
    b @L_80250850
@L_80250424:
    clrlslwi r0, r24, 16, 1
    clrlwi r25, r24, 16
    lhzx r23, r20, r0
    cmplwi r23, 0x0
    beq @L_8025084C
    mr r3, r23
    bl fn_80142984
    clrlwi. r0, r3, 24
    beq @L_8025084C
    mr r3, r23
    mr r4, r29
    bl fn_802126C4
    mr r21, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    beq @L_8025084C
    mr r3, r29
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r5, r3
    mr r6, r23
    addi r3, r1, 0x110
    li r4, 0x0
    li r7, 0x0
    bl fn_801440F0
    extsh r3, r3
    neg r0, r3
    andc r0, r0, r3
    srwi. r0, r0, 31
    beq @L_8025084C
    clrlwi r0, r21, 24
    cmplwi r0, 0x2
    beq @L_802504B8
    cmplwi r0, 0x1
    bne @L_80250530
@L_802504B8:
    mr r3, r28
    mr r4, r29
    bl fn_8023753C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250530
    mr r3, r29
    bl fn_80205B8C
    bl fn_8011FC74
    clrlwi. r0, r3, 24
    bne @L_80250530
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x2e
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x2e
    bl fn_80239EE8
@L_80250530:
    clrlwi r0, r21, 24
    cmplwi r0, 0x3
    beq @L_80250544
    cmplwi r0, 0x1
    bne @L_802505B8
@L_80250544:
    mr r3, r28
    mr r4, r29
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_802505B8
    mr r3, r29
    bl fn_80205B8C
    bl fn_8011FC74
    clrlwi. r0, r3, 24
    bne @L_802505B8
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x2f
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x2f
    bl fn_80239EE8
@L_802505B8:
    clrlwi r0, r21, 24
    cmplwi r0, 0x5
    bne @L_80250624
    mr r3, r28
    mr r4, r29
    bl fn_80235714
    clrlwi. r0, r3, 24
    bne @L_80250624
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x30
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x30
    bl fn_80239EE8
@L_80250624:
    clrlwi r0, r21, 24
    cmplwi r0, 0x4
    bne @L_80250694
    mr r3, r28
    mr r4, r29
    bl fn_80236C80
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    bne @L_80250694
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x31
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x31
    bl fn_80239EE8
@L_80250694:
    clrlwi r0, r21, 24
    cmplwi r0, 0x6
    bne @L_802506F8
    clrlwi r0, r30, 24
    cmplwi r0, 0x1
    bne @L_802506F8
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x32
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x32
    bl fn_80239EE8
@L_802506F8:
    mr r3, r28
    mr r4, r29
    bl fn_8023785C
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    beq @L_80250800
    mr r3, r28
    mr r4, r29
    bl fn_8023785C
    clrlwi r0, r3, 24
    cmplwi r0, 0x3
    beq @L_80250800
    addi r19, r1, 0x60
    clrlwi r17, r26, 16
    li r18, 0x0
    b @L_802507F4
@L_80250738:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    clrlslwi r21, r18, 16, 2
    lwzx r0, r19, r21
    cmplw r3, r0
    beq @L_802507F0
    mr r3, r28
    mr r4, r29
    bl fn_8023785C
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    beq @L_8025078C
    mr r3, r28
    mr r4, r29
    bl fn_8023785C
    clrlwi r0, r3, 24
    cmplwi r0, 0x3
    bne @L_802507F0
@L_8025078C:
    lwzx r3, r19, r21
    bl fn_80206608
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802507F0
    slwi r17, r25, 2
    addi r18, r1, 0xc0
    lwzx r3, r18, r17
    mr r4, r28
    li r5, 0x33
    bl fn_80239984
    stwx r3, r18, r17
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139b
    mr r4, r28
    mr r9, r23
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r10, 0x33
    bl fn_80239EE8
    b @L_80250800
@L_802507F0:
    addi r18, r18, 0x1
@L_802507F4:
    clrlwi r0, r18, 16
    cmplw r0, r17
    blt @L_80250738
@L_80250800:
    mr r3, r29
    bl fn_80205B8C
    stw r23, 0x8(r1)
    li r0, 0x226
    addi r4, r1, 0xc0
    lis r5, 0x1
    stw r0, 0xc(r1)
    slwi r0, r25, 2
    mr r7, r3
    mr r6, r28
    lwzx r0, r4, r0
    subi r3, r5, 0x139b
    subi r4, r5, 0x13ba
    subi r5, r5, 0xdc8
    stw r0, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
@L_8025084C:
    addi r24, r24, 0x1
@L_80250850:
    clrlwi r0, r24, 16
    cmplw r0, r27
    blt @L_80250424
    addi r4, r1, 0xc0
    clrlwi r0, r31, 16
    li r17, 0x0
    b @L_80250880
@L_8025086C:
    clrlslwi r3, r17, 16, 2
    lwzx r3, r4, r3
    cmpwi r3, 0x0
    bgt @L_8025088C
    addi r17, r17, 0x1
@L_80250880:
    clrlwi r3, r17, 16
    cmplw r3, r0
    blt @L_8025086C
@L_8025088C:
    clrlwi r3, r17, 16
    clrlwi r0, r31, 16
    cmplw r3, r0
    blt @L_802508A4
    li r3, 0x0
    b @L_8025096C
@L_802508A4:
    mr r4, r31
    addi r3, r1, 0xc0
    li r5, 0x1
    bl fn_802397B8
    mr. r18, r3
    bge @L_802508C4
    li r3, 0x0
    b @L_8025096C
@L_802508C4:
    clrlslwi r0, r17, 16, 1
    addi r3, r1, 0x38
    lhzx r17, r3, r0
    cmplwi r17, 0x0
    bne @L_802508E0
    li r3, 0x0
    b @L_8025096C
@L_802508E0:
    mr r3, r29
    bl fn_80205B8C
    stw r17, 0x8(r1)
    li r0, 0x228
    addi r4, r1, 0xc0
    lis r5, 0x1
    stw r0, 0xc(r1)
    slwi r0, r18, 2
    mr r7, r3
    mr r6, r28
    lwzx r0, r4, r0
    subi r3, r5, 0x139b
    subi r4, r5, 0x13ba
    subi r5, r5, 0xdc8
    stw r0, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r29
    mr r4, r22
    bl fn_801F0134
    li r0, 0x0
    lis r4, 0x8037
    stw r0, 0x8(r1)
    mr r9, r3
    addi r7, r4, 23920
    mr r3, r29
    mr r8, r17
    li r4, 0x0
    li r5, 0x12
    li r6, 0x0
    li r10, -0x1
    bl fn_80204CE0
    li r3, 0x1
@L_8025096C:
    lmw r17, 0x214(r1)
    lwz r0, 0x254(r1)
    mtlr r0
    addi r1, r1, 0x250
    blr
