    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0xa4(r1)
    stmw r14, 0x58(r1)
    mr r16, r4
    mr r15, r3
    li r4, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r16
    li r24, 0x0
    li r3, 0x0
    bl fn_801F4354
    mr r0, r3
    mr r3, r15
    mr r20, r0
    mr r4, r16
    bl fn_80236E9C
    mr r26, r3
    mr r3, r15
    mr r4, r16
    addi r5, r1, 0x38
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    mr r27, r3
    mr r4, r15
    addi r5, r1, 0x18
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r28, r3
    addi r14, r1, 0x18
    clrlwi r31, r3, 16
    li r18, 0x0
    li r17, 0x0
    li r23, 0x0
    b @L_8024FA50
@L_8024F968:
    clrlslwi r0, r23, 16, 2
    lwzx r19, r14, r0
    cmplwi r19, 0x0
    beq @L_8024FA4C
    mr r3, r15
    mr r4, r19
    bl fn_802376EC
    clrlwi r25, r3, 16
    clrlwi r30, r27, 16
    li r21, 0x0
    b @L_8024FA40
@L_8024F994:
    clrlslwi r0, r21, 16, 1
    addi r3, r1, 0x38
    lhzx r22, r3, r0
    cmplwi r22, 0x0
    beq @L_8024FA3C
    cmplwi r22, 0x165
    beq @L_8024FA3C
    mr r3, r15
    mr r4, r16
    mr r5, r22
    mr r6, r19
    bl fn_8023C530
    mr r29, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F9D8
    li r17, 0x1
@L_8024F9D8:
    mr r3, r15
    mr r4, r19
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F9F4
    li r17, 0x1
@L_8024F9F4:
    mr r3, r15
    mr r4, r22
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    beq @L_8024FA3C
    mr r3, r15
    mr r4, r16
    mr r5, r22
    mr r6, r19
    li r7, 0x0
    bl fn_8023C370
    cmpw r25, r3
    bge @L_8024FA3C
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    bne @L_8024FA3C
    li r18, 0x1
@L_8024FA3C:
    addi r21, r21, 0x1
@L_8024FA40:
    clrlwi r0, r21, 16
    cmplw r0, r30
    blt @L_8024F994
@L_8024FA4C:
    addi r23, r23, 0x1
@L_8024FA50:
    clrlwi r0, r23, 16
    cmplw r0, r31
    blt @L_8024F968
    mr r3, r16
    bl fn_80205B8C
    li r8, 0x0
    lis r5, 0x1
    stw r8, 0x8(r1)
    li r0, 0x227
    mr r7, r3
    mr r6, r20
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13fb
    stw r8, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r15
    mr r4, r16
    bl fn_8023785C
    clrlwi. r0, r3, 24
    beq @L_8024FAFC
    mr r4, r15
    li r3, 0x0
    li r5, 0x1
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1
    bl fn_80239EE8
@L_8024FAFC:
    mr r3, r15
    mr r4, r16
    bl fn_8023753C
    clrlwi. r0, r3, 24
    bne @L_8024FB5C
    mr r3, r24
    mr r4, r15
    li r5, 0x2
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x2
    bl fn_80239EE8
    b @L_8024FBB8
@L_8024FB5C:
    mr r3, r15
    mr r4, r16
    bl fn_8023785C
    clrlwi. r0, r3, 24
    beq @L_8024FBB8
    mr r3, r24
    mr r4, r15
    li r5, 0x3
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x3
    bl fn_80239EE8
@L_8024FBB8:
    mr r3, r15
    mr r4, r16
    bl fn_80235714
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024FC18
    mr r3, r24
    mr r4, r15
    li r5, 0x4
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x4
    bl fn_80239EE8
@L_8024FC18:
    mr r3, r15
    mr r4, r16
    bl fn_8023565C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024FC78
    mr r3, r24
    mr r4, r15
    li r5, 0x5
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x5
    bl fn_80239EE8
@L_8024FC78:
    clrlwi. r0, r17, 24
    bne @L_8024FCC8
    mr r3, r24
    mr r4, r15
    li r5, 0x6
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x6
    bl fn_80239EE8
@L_8024FCC8:
    clrlwi r0, r18, 24
    cmplwi r0, 0x1
    bne @L_8024FD1C
    mr r3, r24
    mr r4, r15
    li r5, 0x7
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x7
    bl fn_80239EE8
@L_8024FD1C:
    mr r3, r15
    mr r4, r16
    bl fn_8023753C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024FDC4
    addi r17, r1, 0x18
    clrlwi r14, r28, 16
    li r18, 0x0
    b @L_8024FDB8
@L_8024FD44:
    clrlslwi r0, r18, 16, 2
    lwzx r4, r17, r0
    cmplwi r4, 0x0
    beq @L_8024FDB4
    mr r3, r15
    bl fn_80236E9C
    clrlwi r3, r3, 16
    clrlwi r0, r26, 16
    cmplw r3, r0
    ble @L_8024FDB4
    mr r3, r24
    mr r4, r15
    li r5, 0x8
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r20
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x8
    bl fn_80239EE8
@L_8024FDB4:
    addi r18, r18, 0x1
@L_8024FDB8:
    clrlwi r0, r18, 16
    cmplw r0, r14
    blt @L_8024FD44
@L_8024FDC4:
    mr r3, r15
    mr r4, r16
    bl fn_80250070
    clrlwi. r0, r3, 16
    mr r14, r3
    beq @L_8024FE24
    mr r3, r24
    mr r4, r15
    mr r5, r14
    bl fn_80239984
    mr r0, r3
    mr r3, r16
    mr r24, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139d
    mr r4, r20
    mr r10, r14
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    bl fn_80239EE8
@L_8024FE24:
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x226
    mr r7, r3
    mr r6, r20
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13fb
    stw r24, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r24
    lmw r14, 0x58(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
