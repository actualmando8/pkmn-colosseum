    nofralloc
    stwu r1, -0x140(r1)
    mflr r0
    stw r0, 0x144(r1)
    stmw r14, 0xf8(r1)
    mr r15, r3
    mr r16, r4
    mr r17, r5
    stw r7, 0x18(r1)
    mr r18, r6
    mr r19, r8
    mr r4, r15
    li r3, 0x3
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r14, r0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r21, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r22, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
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
    rlwinm r25, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r25
    li r5, 0x38
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    mr r4, r16
    stb r0, 0xdc(r1)
    li r3, 0x0
    bl fn_801F4354
    mr r0, r3
    mr r3, r15
    mr r20, r0
    mr r4, r16
    bl fn_80236C80
    mr r23, r3
    mr r3, r14
    mr r4, r21
    mr r5, r22
    mr r6, r24
    bl fn_801F6F38
    addi r6, r1, 0xb4
    addi r5, r1, 0x8c
    addi r4, r1, 0x64
    addi r3, r1, 0x3c
    li r8, 0x0
    li r7, 0x0
    b @L_8023A878
@L_8023A860:
    clrlslwi r0, r8, 16, 2
    addi r8, r8, 0x1
    stwx r7, r6, r0
    stwx r7, r5, r0
    stwx r7, r4, r0
    stwx r7, r3, r0
@L_8023A878:
    rlwinm r0, r8, 0, 16, 31
    cmplwi r0, 0xa
    blt @L_8023A860
    mr r3, r15
    mr r4, r19
    bl fn_802376EC
    lis r4, 0xffff
    rlwinm r24, r3, 0, 16, 31
    addi r22, r4, 0x1
    rlwinm r14, r17, 0, 16, 31
    mr r30, r22
    li r21, 0x0
    li r28, 0x0
    b @L_8023AA14
@L_8023A8B0:
    clrlslwi r0, r28, 16, 1
    rlwinm r26, r28, 0, 16, 31
    lhzx r29, r18, r0
    cmplwi r29, 0x0
    beq @L_8023AA10
    cmplwi r29, 0x165
    beq @L_8023AA10
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
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023A93C
    mr r4, r29
    li r3, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_8011BEB4
    mr. r27, r3
    bne @L_8023A944
    lis r3, -32731
    addi r27, r3, 14664
    b @L_8023A944
@L_8023A93C:
    li r3, 0x0
    b @L_8023A98C
@L_8023A944:
    mr r4, r15
    addi r5, r1, 0x1c
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r4, r29
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    mr r12, r27
    mr r3, r15
    mr r4, r16
    mr r5, r29
    mr r6, r19
    mtctr r12
    bctrl
@L_8023A98C:
    rlwinm r26, r26, 2, 0, 29
    addi r5, r1, 0x8c
    stwx r3, r5, r26
    addi r31, r1, 0x64
    mr r3, r15
    mr r4, r29
    lwzx r27, r5, r26
    li r5, 0x1
    subf r0, r27, r24
    stwx r0, r31, r26
    bl fn_8023943C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023AA10
    cmpw r22, r27
    bge @L_8023A9D0
    mr r22, r27
@L_8023A9D0:
    lwzx r0, r31, r26
    cmpwi r0, 0x0
    bgt @L_8023AA10
    cmpw r30, r0
    bge @L_8023A9E8
    mr r30, r0
@L_8023A9E8:
    mr r3, r15
    mr r4, r29
    bl fn_802393A0
    rlwinm r0, r3, 0, 24, 31
    addi r3, r1, 0x3c
    stwx r0, r3, r26
    lwzx r0, r3, r26
    cmpw r21, r0
    bge @L_8023AA10
    mr r21, r0
@L_8023AA10:
    addi r28, r28, 0x1
@L_8023AA14:
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r14
    blt @L_8023A8B0
    lbz r0, 0xdc(r1)
    li r29, 0x0
    clrlslwi r3, r0, 24, 1
    rlwinm r0, r23, 0, 24, 31
    sth r0, 0xe4(r1)
    addi r0, r3, 0x1
    stw r0, 0xe0(r1)
    rlwinm r0, r17, 0, 16, 31
    stw r0, 0xec(r1)
    b @L_8023B3FC
@L_8023AA48:
    clrlslwi r0, r29, 16, 1
    rlwinm r26, r29, 0, 16, 31
    lhzx r28, r18, r0
    cmplwi r28, 0x0
    beq @L_8023B3F8
    lwz r3, 0x18(r1)
    lhax r24, r3, r0
    cmpwi r24, 0x0
    blt @L_8023B3F8
    mr r3, r15
    mr r4, r28
    mr r5, r16
    bl fn_802395C8
    mr r0, r3
    mr r3, r15
    mr r14, r0
    mr r4, r28
    bl fn_80239244
    stw r3, 0xe8(r1)
    mr r4, r28
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    mr r23, r3
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
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023AB1C
    mr r4, r28
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_8023AB24
    lis r3, -32731
    addi r3, r3, -6868
    b @L_8023AB24
@L_8023AB1C:
    li r3, 0x0
    b @L_8023AB40
@L_8023AB24:
    mr r12, r3
    mr r3, r15
    mr r4, r16
    mr r5, r28
    mr r6, r19
    mtctr r12
    bctrl
@L_8023AB40:
    rlwinm r30, r26, 2, 0, 29
    addi r31, r1, 0xb4
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x227
    mr r7, r3
    mr r6, r20
    stw r0, 0xc(r1)
    mr r10, r28
    subi r3, r5, 0x139c
    subi r4, r5, 0x13d4
    lwzx r0, r31, r30
    subi r5, r5, 0x13ce
    li r8, 0x0
    li r9, 0x0
    stw r0, 0x10(r1)
    bl fn_8023A118
    mr r3, r15
    mr r4, r16
    mr r5, r28
    mr r6, r19
    bl fn_8023C530
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023AC54
    lis r3, -32732
    addi r0, r3, 15452
    cmplw r23, r0
    bne @L_8023AC0C
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x3e
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x3e
    bl fn_80239EE8
    b @L_8023AC98
@L_8023AC0C:
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x3c
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x3c
    bl fn_80239EE8
    b @L_8023AC98
@L_8023AC54:
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x3d
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x3d
    bl fn_80239EE8
@L_8023AC98:
    rlwinm r0, r14, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_8023ADA0
    mr r3, r15
    mr r4, r28
    li r5, 0x1
    bl fn_8023943C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023ADA0
    mr r3, r15
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r15
    mr r4, r19
    mr r5, r14
    bl fn_8023793C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x42
    bne @L_8023AD30
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x3f
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x3f
    bl fn_80239EE8
@L_8023AD30:
    mr r3, r15
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r15
    mr r4, r19
    mr r5, r14
    bl fn_8023793C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x43
    bne @L_8023ADA0
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x40
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x40
    bl fn_80239EE8
@L_8023ADA0:
    li r23, 0x0
    b @L_8023AE24
@L_8023ADA8:
    mr r3, r15
    mr r4, r28
    rlwinm r5, r23, 0, 24, 31
    bl fn_80239498
    rlwinm. r6, r3, 0, 24, 31
    beq @L_8023AE20
    lhz r4, 0xe4(r1)
    li r3, 0x0
    li r5, 0xc
    bl fn_801FB1C0
    rlwinm r26, r3, 0, 24, 31
    lwzx r3, r31, r30
    mr r4, r26
    mr r5, r15
    li r6, 0x41
    bl fn_802398E4
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    stw r26, 0x8(r1)
    mr r5, r3
    mr r4, r15
    subi r3, r6, 0x139c
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x41
    bl fn_80239A40
@L_8023AE20:
    addi r23, r23, 0x1
@L_8023AE24:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x3
    blt @L_8023ADA8
    rlwinm r0, r14, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_8023AEE4
    li r23, 0x0
    b @L_8023AED8
@L_8023AE44:
    mr r4, r25
    mr r6, r23
    li r3, 0x0
    li r5, 0x39
    bl fn_801FB1C0
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x9
    beq @L_8023AED4
    rlwinm r0, r14, 0, 16, 31
    cmplw r0, r3
    bne @L_8023AED4
    mr r4, r25
    mr r6, r23
    li r3, 0x0
    li r5, 0x3a
    bl fn_801FB1C0
    rlwinm r26, r3, 0, 24, 31
    lwzx r3, r31, r30
    mr r4, r26
    mr r5, r15
    li r6, 0x42
    bl fn_802398E4
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    stw r26, 0x8(r1)
    mr r5, r3
    mr r4, r15
    subi r3, r6, 0x139c
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x42
    bl fn_80239A40
@L_8023AED4:
    addi r23, r23, 0x1
@L_8023AED8:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8023AE44
@L_8023AEE4:
    li r27, 0x0
    b @L_8023AFB0
@L_8023AEEC:
    mr r3, r15
    mr r4, r28
    rlwinm r5, r27, 0, 24, 31
    bl fn_80239498
    rlwinm. r0, r3, 0, 24, 31
    mr r14, r3
    beq @L_8023AFAC
    li r23, 0x0
    b @L_8023AFA0
@L_8023AF10:
    mr r4, r25
    mr r6, r23
    li r3, 0x0
    li r5, 0x3b
    bl fn_801FB1C0
    rlwinm. r3, r3, 0, 24, 31
    beq @L_8023AF9C
    rlwinm r0, r14, 0, 24, 31
    cmplw r0, r3
    bne @L_8023AF9C
    mr r4, r25
    mr r6, r23
    li r3, 0x0
    li r5, 0x3c
    bl fn_801FB1C0
    rlwinm r26, r3, 0, 24, 31
    lwzx r3, r31, r30
    mr r4, r26
    mr r5, r15
    li r6, 0x43
    bl fn_802398E4
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    stw r26, 0x8(r1)
    mr r5, r3
    mr r4, r15
    subi r3, r6, 0x139c
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x43
    bl fn_80239A40
@L_8023AF9C:
    addi r23, r23, 0x1
@L_8023AFA0:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8023AF10
@L_8023AFAC:
    addi r27, r27, 0x1
@L_8023AFB0:
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x3
    blt @L_8023AEEC
    mr r3, r15
    mr r4, r28
    li r5, 0x1
    bl fn_8023943C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B128
    addi r3, r1, 0x8c
    lwzx r0, r3, r30
    cmpw r22, r0
    bgt @L_8023B02C
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x44
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x44
    bl fn_80239EE8
@L_8023B02C:
    addi r3, r1, 0x64
    lwzx r14, r3, r30
    cmpwi r14, 0x0
    bge @L_8023B080
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x45
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x45
    bl fn_80239EE8
@L_8023B080:
    cmpwi r14, 0x0
    bgt @L_8023B0CC
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x46
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x46
    bl fn_80239EE8
@L_8023B0CC:
    cmpwi r14, 0x0
    bgt @L_8023B128
    addi r3, r1, 0x3c
    lwzx r0, r3, r30
    cmpw r21, r0
    bgt @L_8023B128
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x47
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x47
    bl fn_80239EE8
@L_8023B128:
    mr r3, r15
    mr r4, r28
    li r5, 0x2
    bl fn_8023943C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B1A0
    mr r3, r15
    mr r4, r16
    bl fn_8023753C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B1A0
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x48
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x48
    bl fn_80239EE8
@L_8023B1A0:
    mr r3, r15
    mr r4, r28
    bl fn_802392A8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B1FC
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x49
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x49
    bl fn_80239EE8
@L_8023B1FC:
    lwz r0, 0xe8(r1)
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x4
    bne @L_8023B270
    mr r5, r19
    li r3, 0x0
    li r4, 0x1
    li r6, 0x1
    bl fn_801F2654
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8023B270
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x4a
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x4a
    bl fn_80239EE8
@L_8023B270:
    mr r3, r15
    mr r4, r16
    rlwinm r5, r24, 0, 24, 31
    bl fn_80238060
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B2D0
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x4b
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x4b
    bl fn_80239EE8
@L_8023B2D0:
    mr r3, r15
    mr r4, r28
    bl fn_80239154
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B32C
    lwzx r3, r31, r30
    mr r4, r15
    li r5, 0x4c
    bl fn_80239984
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r20
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x4c
    bl fn_80239EE8
@L_8023B32C:
    bl fn_8000815C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023B3B0
    bl fn_800E0C54
    lwz r0, 0xe0(r1)
    rlwinm r5, r3, 0, 16, 31
    lwzx r3, r31, r30
    divw r4, r5, r0
    mullw r0, r4, r0
    subf r4, r0, r5
    lbz r0, 0xdc(r1)
    subf r14, r0, r4
    mr r4, r14
    bl fn_802399FC
    stwx r3, r31, r30
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x225
    mr r7, r3
    mr r6, r20
    stw r0, 0xc(r1)
    mr r10, r28
    subi r3, r5, 0x139c
    subi r4, r5, 0x13d4
    stw r14, 0x10(r1)
    subi r5, r5, 0x13ce
    li r8, 0x0
    li r9, 0x0
    bl fn_8023A118
@L_8023B3B0:
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x226
    mr r7, r3
    mr r6, r20
    stw r0, 0xc(r1)
    mr r10, r28
    subi r3, r5, 0x139c
    subi r4, r5, 0x13d4
    lwzx r0, r31, r30
    subi r5, r5, 0x13ce
    li r8, 0x0
    li r9, 0x0
    stw r0, 0x10(r1)
    bl fn_8023A118
@L_8023B3F8:
    addi r29, r29, 0x1
@L_8023B3FC:
    lwz r0, 0xec(r1)
    rlwinm r3, r29, 0, 16, 31
    cmplw r3, r0
    blt @L_8023AA48
    mr r4, r17
    addi r3, r1, 0xb4
    li r5, 0x1
    bl fn_802397B8
    mr. r14, r3
    bge @L_8023B42C
    li r3, -0x1
    b @L_8023B484
@L_8023B42C:
    mr r3, r16
    bl fn_80205B8C
    li r0, 0x0
    rlwinm r7, r14, 2, 0, 29
    stw r0, 0x8(r1)
    li r0, 0x228
    addi r4, r1, 0xb4
    lis r5, 0x1
    stw r0, 0xc(r1)
    rlwinm r0, r14, 1, 0, 30
    mr r6, r20
    li r9, 0x0
    lwzx r8, r4, r7
    mr r7, r3
    subi r3, r5, 0x139c
    subi r4, r5, 0x13d4
    stw r8, 0x10(r1)
    subi r5, r5, 0x13ce
    li r8, 0x0
    lhzx r10, r18, r0
    bl fn_8023A118
    mr r3, r14
@L_8023B484:
    lmw r14, 0xf8(r1)
    lwz r0, 0x144(r1)
    mtlr r0
    addi r1, r1, 0x140
    blr
