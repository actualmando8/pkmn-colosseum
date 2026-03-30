    nofralloc
    stwu r1, -0x860(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x43
    stw r0, 0x864(r1)
    stmw r14, 0x818(r1)
    mr r16, r6
    mr r15, r3
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r21, r3, 16
    li r3, 0x0
    mr r4, r21
    li r5, 0x38
    li r6, 0x0
    bl fn_801FB1C0
    lis r5, lbl_8027A434@ha
    clrlwi r0, r3, 24
    addi r5, r5, lbl_8027A434@l
    li r4, 0x3
    stb r0, 0x7f0(r1)
    addi r6, r1, 0x14
    li r14, 0x0
    subi r5, r5, 0x4
    mtctr r4
@L_8024E708:
    lwz r3, 0x4(r5)
    lwzu r0, 0x8(r5)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8024E708
    lwz r0, 0x4(r5)
    mr r3, r15
    li r4, 0x0
    li r5, 0x1
    stw r0, 0x4(r6)
    bl fn_80235B04
    stw r3, 0x7f8(r1)
    addi r3, r1, 0x68
    li r5, 0x0
    li r4, 0x0
    b @L_8024E754
@L_8024E748:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
@L_8024E754:
    clrlwi r0, r5, 16
    cmplwi r0, 0x6
    blt @L_8024E748
    mr r4, r16
    addi r3, r1, 0x110
    bl fn_801FCEC4
    mr r3, r16
    bl fn_802062FC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024E7A0
    mr r3, r16
    li r4, 0xe2
    li r5, 0x0
    bl fn_80204DE4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024E7A0
    li r14, 0x1
@L_8024E7A0:
    mr r3, r15
    addi r4, r1, 0x98
    bl fn_801F87CC
    mr r20, r3
    mr r4, r15
    addi r5, r1, 0x34
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    stw r3, 0x7fc(r1)
    mr r4, r15
    addi r5, r1, 0xb0
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    clrlwi. r0, r20, 16
    stw r3, 0x800(r1)
    bne @L_8024E7F8
    li r3, -0x1
    b @L_8024F8A0
@L_8024E7F8:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024E8F0
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    mr r6, r15
    subi r3, r5, 0x139e
    subi r4, r5, 0x13fc
    stw r0, 0xc(r1)
    subi r5, r5, 0x13e3
    li r7, 0x0
    li r8, 0x0
    stw r0, 0x10(r1)
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    bl fn_800E0C54
    clrlwi r5, r3, 16
    clrlwi r4, r20, 16
    divw r0, r5, r4
    addi r3, r1, 0x98
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r17, r3, r0
    cmplwi r17, 0x0
    beq @L_8024E8F0
    mr r4, r17
    li r3, 0x0
    bl fn_801F4460
    mr r0, r3
    mr r3, r17
    mr r14, r0
    bl fn_80205BE8
    li r8, 0x0
    lis r5, 0x1
    stw r8, 0x8(r1)
    li r0, 0x228
    mr r7, r3
    mr r6, r14
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    stw r8, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r17
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
    b @L_8024F8A0
@L_8024E8F0:
    clrlwi r0, r14, 24
    cmplwi r0, 0x1
    bne @L_8024EA84
    addi r14, r1, 0x98
    addi r17, r1, 0x80
    addi r18, r1, 0x18
    li r19, 0x0
    li r26, 0x0
    b @L_8024E9D4
@L_8024E914:
    clrlslwi r3, r26, 16, 1
    li r4, 0x0
    addi r0, r3, 0x2
    lhax r23, r18, r3
    lhax r24, r18, r0
    mr r3, r4
    b @L_8024E93C
@L_8024E930:
    clrlslwi r0, r4, 16, 2
    addi r4, r4, 0x1
    stwx r3, r17, r0
@L_8024E93C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x6
    blt @L_8024E930
    li r22, 0x0
    mr r25, r22
    b @L_8024E9B8
@L_8024E954:
    clrlslwi r0, r25, 16, 2
    lwzx r19, r14, r0
    cmplwi r19, 0x0
    beq @L_8024E9B4
    extsh. r0, r23
    blt @L_8024E984
    mr r3, r15
    mr r4, r19
    bl fn_80238600
    clrlwi r0, r3, 24
    cmpw r23, r0
    bne @L_8024E9B4
@L_8024E984:
    extsh. r0, r24
    blt @L_8024E9A4
    mr r3, r15
    mr r4, r19
    bl fn_80238538
    clrlwi r0, r3, 24
    cmpw r24, r0
    bne @L_8024E9B4
@L_8024E9A4:
    clrlslwi r0, r22, 16, 2
    addi r3, r1, 0x80
    stwx r19, r3, r0
    addi r22, r22, 0x1
@L_8024E9B4:
    addi r25, r25, 0x1
@L_8024E9B8:
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8024E954
    clrlwi. r0, r22, 16
    mr r19, r22
    bne @L_8024E9E0
    addi r26, r26, 0x2
@L_8024E9D4:
    clrlwi r0, r26, 16
    cmplwi r0, 0xe
    blt @L_8024E914
@L_8024E9E0:
    clrlwi. r0, r19, 16
    beq @L_8024EA84
    bl fn_800E0C54
    clrlwi r5, r3, 16
    clrlwi r4, r19, 16
    divw r0, r5, r4
    addi r3, r1, 0x80
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r14, r3, r0
    cmplwi r14, 0x0
    beq @L_8024EA84
    mr r4, r14
    li r3, 0x0
    bl fn_801F4460
    mr r15, r3
    mr r3, r14
    bl fn_80205BE8
    li r8, 0x0
    lis r5, 0x1
    stw r8, 0x8(r1)
    li r0, 0x228
    mr r7, r3
    mr r6, r15
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    stw r8, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r14
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
    b @L_8024F8A0
@L_8024EA84:
    lis r4, 0xffff
    lis r3, 0x1
    addi r18, r4, 0x1
    addi r22, r1, 0x98
    subi r14, r3, 0x1
    clrlwi r17, r20, 16
    li r19, 0x0
    li r23, 0x0
    b @L_8024EB44
@L_8024EAA8:
    clrlslwi r0, r23, 16, 2
    lwzx r24, r22, r0
    cmplwi r24, 0x0
    beq @L_8024EB40
    mr r3, r24
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh. r0, r3
    blt @L_8024EB40
    mr r3, r15
    mr r4, r24
    bl fn_802386C8
    mr r0, r3
    mr r3, r15
    mr r25, r0
    mr r4, r24
    bl fn_802389D4
    mr r0, r3
    mr r3, r24
    mr r24, r0
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    cmplw r19, r25
    clrlwi r3, r3, 16
    bge @L_8024EB24
    mr r19, r25
@L_8024EB24:
    cmpw r18, r24
    bge @L_8024EB30
    mr r18, r24
@L_8024EB30:
    clrlwi r0, r14, 16
    cmplw r0, r3
    ble @L_8024EB40
    mr r14, r3
@L_8024EB40:
    addi r23, r23, 0x1
@L_8024EB44:
    clrlwi r0, r23, 16
    cmplw r0, r17
    blt @L_8024EAA8
    mr r4, r21
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024EC60
    addi r22, r1, 0x98
    clrlwi r17, r20, 16
    li r23, 0x0
    b @L_8024EC54
@L_8024EB80:
    clrlslwi r0, r23, 16, 2
    lwzx r24, r22, r0
    cmplwi r24, 0x0
    beq @L_8024EC50
    mr r3, r24
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh. r0, r3
    blt @L_8024EC50
    mr r4, r24
    li r3, 0x0
    bl fn_801F4460
    mr r3, r24
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 16
    clrlwi r0, r14, 16
    cmplw r0, r3
    blt @L_8024EC50
    mr r4, r24
    li r3, 0x0
    bl fn_801F4460
    mr r14, r3
    mr r3, r24
    bl fn_80205BE8
    li r8, 0x0
    lis r5, 0x1
    stw r8, 0x8(r1)
    li r0, 0x228
    mr r7, r3
    mr r6, r14
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    stw r8, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r24
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
    b @L_8024F8A0
@L_8024EC50:
    addi r23, r23, 0x1
@L_8024EC54:
    clrlwi r0, r23, 16
    cmplw r0, r17
    blt @L_8024EB80
@L_8024EC60:
    lbz r0, 0x7f0(r1)
    li r28, 0x0
    clrlslwi r3, r0, 24, 1
    addi r0, r3, 0x1
    stw r0, 0x7f4(r1)
    clrlwi r0, r20, 16
    stw r0, 0x808(r1)
    b @L_8024F7E0
@L_8024EC80:
    clrlslwi r30, r28, 16, 2
    addi r3, r1, 0x98
    lwzx r27, r3, r30
    cmplwi r27, 0x0
    beq @L_8024F7DC
    mr r3, r27
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh. r0, r3
    blt @L_8024F7DC
    mr r4, r27
    li r3, 0x0
    bl fn_801F4460
    mr r26, r3
    mr r3, r16
    mr r4, r27
    li r5, 0x0
    bl fn_802068C8
    mr r3, r15
    mr r4, r16
    addi r5, r1, 0x54
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    lwz r0, 0x7fc(r1)
    mr r23, r3
    addi r31, r1, 0x34
    li r14, 0x0
    clrlwi r17, r0, 16
    li r25, 0x0
    b @L_8024ED80
@L_8024ED04:
    clrlslwi r0, r25, 16, 2
    lwzx r29, r31, r0
    cmplwi r29, 0x0
    beq @L_8024ED7C
    clrlwi r22, r23, 16
    li r24, 0x0
    b @L_8024ED64
@L_8024ED20:
    clrlslwi r0, r24, 16, 1
    addi r3, r1, 0x54
    lhzx r5, r3, r0
    cmplwi r5, 0x0
    beq @L_8024ED60
    cmplwi r5, 0x165
    beq @L_8024ED60
    mr r3, r15
    mr r4, r16
    mr r6, r29
    bl fn_8023C530
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024ED60
    li r14, 0x1
    b @L_8024ED70
@L_8024ED60:
    addi r24, r24, 0x1
@L_8024ED64:
    clrlwi r0, r24, 16
    cmplw r0, r22
    blt @L_8024ED20
@L_8024ED70:
    clrlwi r0, r14, 24
    cmplwi r0, 0x1
    beq @L_8024ED8C
@L_8024ED7C:
    addi r25, r25, 0x1
@L_8024ED80:
    clrlwi r0, r25, 16
    cmplw r0, r17
    blt @L_8024ED04
@L_8024ED8C:
    mr r3, r16
    addi r4, r1, 0x110
    bl fn_801FCEC4
    mr r3, r15
    mr r4, r27
    addi r5, r1, 0x54
    li r6, 0x0
    li r7, 0x1
    bl fn_802369B8
    stw r3, 0x804(r1)
    mr r3, r27
    bl fn_80205BE8
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x227
    mr r7, r3
    addi r29, r1, 0x68
    stw r0, 0xc(r1)
    mr r6, r26
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    lwzx r0, r29, r30
    subi r5, r5, 0x13e3
    li r8, 0x0
    li r9, 0x0
    stw r0, 0x10(r1)
    li r10, 0x0
    bl fn_8023A118
    mr r3, r15
    mr r4, r27
    bl fn_80238600
    mr r17, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024EE60
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x1a
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1a
    bl fn_80239EE8
@L_8024EE60:
    clrlwi r0, r17, 24
    cmplwi r0, 0x2
    bne @L_8024EEB0
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x1b
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1b
    bl fn_80239EE8
@L_8024EEB0:
    clrlwi r0, r17, 24
    cmplwi r0, 0x3
    bne @L_8024EF00
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x1c
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1c
    bl fn_80239EE8
@L_8024EF00:
    li r17, 0x0
    b @L_8024EFA8
@L_8024EF08:
    mr r4, r21
    mr r6, r17
    li r3, 0x0
    li r5, 0x39
    bl fn_801FB1C0
    clrlwi r5, r3, 16
    cmplwi r5, 0x9
    beq @L_8024EFA4
    mr r3, r15
    mr r4, r27
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024EFA4
    mr r4, r21
    mr r6, r17
    li r3, 0x0
    li r5, 0x3a
    bl fn_801FB1C0
    clrlwi r22, r3, 24
    lwzx r3, r29, r30
    mr r4, r22
    mr r5, r15
    li r6, 0x1d
    bl fn_802398E4
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r6, 0x1
    stw r22, 0x8(r1)
    mr r5, r3
    mr r4, r15
    subi r3, r6, 0x139d
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1d
    bl fn_80239A40
@L_8024EFA4:
    addi r17, r17, 0x1
@L_8024EFA8:
    clrlwi r0, r17, 16
    cmplwi r0, 0x2
    blt @L_8024EF08
    mr r3, r15
    mr r4, r27
    bl fn_8023881C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F010
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x1e
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1e
    bl fn_80239EE8
@L_8024F010:
    clrlwi. r0, r14, 24
    bne @L_8024F05C
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x1f
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1f
    bl fn_80239EE8
@L_8024F05C:
    mr r3, r15
    mr r4, r27
    bl fn_802386C8
    cmplw r19, r3
    bgt @L_8024F0B4
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x21
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x21
    bl fn_80239EE8
@L_8024F0B4:
    mr r3, r15
    mr r4, r27
    bl fn_802389D4
    cmpw r18, r3
    bgt @L_8024F10C
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x20
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x20
    bl fn_80239EE8
@L_8024F10C:
    lwz r0, 0x7fc(r1)
    li r24, 0x0
    clrlwi r14, r0, 16
    b @L_8024F210
@L_8024F11C:
    clrlslwi r0, r24, 16, 2
    addi r3, r1, 0x34
    lwzx r23, r3, r0
    cmplwi r23, 0x0
    beq @L_8024F20C
    lwz r0, 0x804(r1)
    li r25, 0x0
    clrlwi r31, r0, 16
    b @L_8024F200
@L_8024F140:
    clrlslwi r0, r25, 16, 1
    addi r3, r1, 0x54
    lhzx r22, r3, r0
    cmplwi r22, 0x0
    beq @L_8024F1FC
    mr r3, r15
    mr r4, r22
    mr r5, r16
    bl fn_802395C8
    clrlwi r0, r3, 16
    mr r17, r3
    cmplwi r0, 0x9
    beq @L_8024F1FC
    mr r3, r15
    mr r4, r22
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    beq @L_8024F1FC
    mr r3, r15
    mr r4, r22
    bl fn_80239500
    mr r6, r3
    mr r3, r15
    mr r4, r23
    mr r5, r17
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_8024F1FC
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x22
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x22
    bl fn_80239EE8
@L_8024F1FC:
    addi r25, r25, 0x1
@L_8024F200:
    clrlwi r0, r25, 16
    cmplw r0, r31
    blt @L_8024F140
@L_8024F20C:
    addi r24, r24, 0x1
@L_8024F210:
    clrlwi r0, r24, 16
    cmplw r0, r14
    blt @L_8024F11C
    lwz r0, 0x7fc(r1)
    li r22, 0x0
    clrlwi r31, r0, 16
    b @L_8024F334
@L_8024F22C:
    clrlslwi r0, r22, 16, 2
    addi r3, r1, 0x34
    lwzx r23, r3, r0
    cmplwi r23, 0x0
    beq @L_8024F330
    mr r3, r15
    mr r4, r23
    addi r5, r1, 0x54
    li r6, 0x0
    li r7, 0x0
    bl fn_802367CC
    clrlwi r14, r3, 16
    li r17, 0x0
    b @L_8024F324
@L_8024F264:
    clrlslwi r0, r17, 16, 1
    addi r3, r1, 0x54
    lhzx r24, r3, r0
    cmplwi r24, 0x0
    beq @L_8024F320
    mr r3, r15
    mr r4, r24
    mr r5, r23
    bl fn_802395C8
    clrlwi r0, r3, 16
    mr r25, r3
    cmplwi r0, 0x9
    beq @L_8024F320
    mr r3, r15
    mr r4, r24
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    beq @L_8024F320
    mr r3, r15
    mr r4, r24
    bl fn_80239500
    mr r6, r3
    mr r3, r15
    mr r4, r27
    mr r5, r25
    bl fn_80238B0C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_8024F320
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x23
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x23
    bl fn_80239EE8
@L_8024F320:
    addi r17, r17, 0x1
@L_8024F324:
    clrlwi r0, r17, 16
    cmplw r0, r14
    blt @L_8024F264
@L_8024F330:
    addi r22, r22, 0x1
@L_8024F334:
    clrlwi r0, r22, 16
    cmplw r0, r31
    blt @L_8024F22C
    mr r4, r21
    li r3, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F478
    li r17, 0x0
    b @L_8024F468
@L_8024F368:
    clrlwi r0, r17, 16
    cmplwi r0, 0x9
    beq @L_8024F464
    mr r3, r15
    mr r4, r27
    mr r5, r17
    li r6, 0x1
    bl fn_80238B0C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_8024F464
    lwz r0, 0x800(r1)
    addi r14, r1, 0xb0
    li r22, 0x0
    clrlwi r23, r0, 16
    b @L_8024F458
@L_8024F3A8:
    clrlslwi r0, r22, 16, 2
    lwzx r24, r14, r0
    cmplwi r24, 0x0
    beq @L_8024F454
    mr r4, r24
    li r3, 0x0
    bl fn_801F4460
    cmplwi r3, 0x0
    beq @L_8024F454
    mr r4, r24
    bl fn_801F8C00
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024F454
    cmplwi r0, 0x2
    beq @L_8024F3F0
    cmplwi r0, 0x3
    bne @L_8024F454
@L_8024F3F0:
    mr r3, r15
    mr r4, r24
    mr r5, r17
    li r6, 0x1
    bl fn_80238B0C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_8024F454
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x24
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x24
    bl fn_80239EE8
@L_8024F454:
    addi r22, r22, 0x1
@L_8024F458:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_8024F3A8
@L_8024F464:
    addi r17, r17, 0x1
@L_8024F468:
    lwz r0, -31976(r13)
    clrlwi r3, r17, 16
    cmplw r3, r0
    blt @L_8024F368
@L_8024F478:
    mr r3, r15
    mr r4, r27
    bl fn_8024FE80
    clrlwi. r0, r3, 16
    mr r14, r3
    beq @L_8024F4D4
    lwzx r3, r29, r30
    mr r4, r15
    mr r5, r14
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139d
    mr r4, r26
    mr r10, r14
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    bl fn_80239EE8
@L_8024F4D4:
    lwz r0, 0x7f8(r1)
    clrlwi r0, r0, 24
    cmplwi r0, 0x2
    bne @L_8024F564
    mr r3, r15
    mr r4, r27
    li r5, 0x21
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024F51C
    mr r3, r15
    mr r4, r27
    li r5, 0x2c
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F710
@L_8024F51C:
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x29
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x29
    bl fn_80239EE8
    b @L_8024F710
@L_8024F564:
    cmplwi r0, 0x1
    bne @L_8024F5D0
    mr r3, r15
    mr r4, r27
    li r5, 0x22
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F710
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x2a
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x2a
    bl fn_80239EE8
    b @L_8024F710
@L_8024F5D0:
    cmplwi r0, 0x3
    bne @L_8024F6A8
    mr r3, r15
    mr r4, r27
    li r14, 0x0
    li r5, 0x8
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F5FC
    li r14, 0x1
@L_8024F5FC:
    mr r3, r15
    mr r4, r27
    li r5, 0x8
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024F650
    mr r3, r15
    mr r4, r27
    li r5, 0x5
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024F650
    mr r3, r15
    mr r4, r27
    li r5, 0x4
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F654
@L_8024F650:
    li r14, 0x1
@L_8024F654:
    clrlwi r0, r14, 24
    cmplwi r0, 0x1
    bne @L_8024F710
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x2b
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x2b
    bl fn_80239EE8
    b @L_8024F710
@L_8024F6A8:
    cmplwi r0, 0x4
    bne @L_8024F710
    mr r3, r15
    mr r4, r27
    li r5, 0xf
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F710
    lwzx r3, r29, r30
    mr r4, r15
    li r5, 0x2c
    bl fn_80239984
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    lis r7, 0x1
    mr r5, r3
    subi r3, r7, 0x139d
    mr r4, r26
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x2c
    bl fn_80239EE8
@L_8024F710:
    bl fn_8000815C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024F794
    bl fn_800E0C54
    lwz r0, 0x7f4(r1)
    clrlwi r5, r3, 16
    lwzx r3, r29, r30
    divw r4, r5, r0
    mullw r0, r4, r0
    subf r4, r0, r5
    lbz r0, 0x7f0(r1)
    subf r14, r0, r4
    mr r4, r14
    bl fn_802399FC
    stwx r3, r29, r30
    mr r3, r27
    bl fn_80205BE8
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x225
    mr r7, r3
    mr r6, r26
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    stw r14, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
@L_8024F794:
    mr r3, r27
    bl fn_80205BE8
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x226
    mr r7, r3
    mr r6, r26
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    lwzx r0, r29, r30
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    stw r0, 0x10(r1)
    bl fn_8023A118
@L_8024F7DC:
    addi r28, r28, 0x1
@L_8024F7E0:
    lwz r0, 0x808(r1)
    clrlwi r3, r28, 16
    cmplw r3, r0
    blt @L_8024EC80
    mr r4, r20
    addi r3, r1, 0x68
    li r5, 0x1
    bl fn_802397B8
    cmpwi r3, 0x0
    bge @L_8024F810
    li r3, -0x1
    b @L_8024F8A0
@L_8024F810:
    slwi r14, r3, 2
    addi r3, r1, 0x98
    lwzx r15, r3, r14
    cmplwi r15, 0x0
    bne @L_8024F82C
    li r3, -0x1
    b @L_8024F8A0
@L_8024F82C:
    mr r4, r15
    li r3, 0x0
    bl fn_801F4460
    mr r16, r3
    mr r3, r15
    bl fn_80205BE8
    li r0, 0x0
    addi r4, r1, 0x68
    stw r0, 0x8(r1)
    li r0, 0x228
    lis r5, 0x1
    mr r7, r3
    stw r0, 0xc(r1)
    mr r6, r16
    subi r3, r5, 0x139d
    li r8, 0x0
    lwzx r0, r4, r14
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13e3
    li r9, 0x0
    stw r0, 0x10(r1)
    li r10, 0x0
    bl fn_8023A118
    mr r3, r15
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
@L_8024F8A0:
    lmw r14, 0x818(r1)
    lwz r0, 0x864(r1)
    mtlr r0
    addi r1, r1, 0x860
    blr
