    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x84(r1)
    li r0, 0x0
    li r5, 0xe
    li r6, 0x0
    stmw r24, 0x60(r1)
    stb r0, 0x8(r1)
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    li r3, 0xb
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r25, r3
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r25
    mr r25, r3
    li r3, 0x9
    bl fn_801F025C
    li r4, 0x0
    mr r29, r3
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    mr r3, r29
    mr r26, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r24, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_801F54A4
    mr r30, r3
    mr r4, r27
    li r3, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E0F8
    bl fn_8020DFC0
    mr r31, r3
    mr r3, r29
    mr r4, r24
    li r5, 0x7
    li r6, 0x0
    bl fn_801FB1C0
    mr. r28, r3
    bne @L_8020C24C
    li r28, 0x5f
@L_8020C24C:
    mr r4, r24
    li r3, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    li r3, 0x0
    mr r27, r0
    bl fn_801F1888
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020C69C
    cmplwi r30, 0x0
    beq @L_8020C430
    cmplwi r31, 0x0
    beq @L_8020C298
    mr r3, r26
    rlwinm r4, r31, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_8020C298:
    mr r3, r26
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_8020C2A8:
    addic. r0, r1, 0x8
    bne @L_8020C2B8
    li r0, 0x0
    b @L_8020C2EC
@L_8020C2B8:
    addi r3, r1, 0x44
    li r4, 0x1
    bl fn_80103BA8
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_8020C2E8
    lhz r0, 0x44(r1)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_8020C2E8
    li r0, 0x1
    stb r0, 0x8(r1)
@L_8020C2E8:
    lbz r0, 0x8(r1)
@L_8020C2EC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8020C430
    mr r3, r26
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C318
    bl fn_800F0308
    b @L_8020C2A8
@L_8020C318:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C3B8
    lhz r4, -21032(r13)
    mr r3, r26
    li r5, 0x4
    bl fn_801DA9E8
@L_8020C348:
    addic. r0, r1, 0x8
    bne @L_8020C358
    li r0, 0x0
    b @L_8020C38C
@L_8020C358:
    addi r3, r1, 0x28
    li r4, 0x1
    bl fn_80103BA8
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_8020C388
    lhz r0, 0x28(r1)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_8020C388
    li r0, 0x1
    stb r0, 0x8(r1)
@L_8020C388:
    lbz r0, 0x8(r1)
@L_8020C38C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8020C430
    lhz r4, -21032(r13)
    mr r3, r26
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C3B8
    bl fn_800F0308
    b @L_8020C348
@L_8020C3B8:
    cmplwi r31, 0x0
    beq @L_8020C430
@L_8020C3C0:
    addic. r0, r1, 0x8
    bne @L_8020C3D0
    li r0, 0x0
    b @L_8020C404
@L_8020C3D0:
    addi r3, r1, 0xc
    li r4, 0x1
    bl fn_80103BA8
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_8020C400
    lhz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_8020C400
    li r0, 0x1
    stb r0, 0x8(r1)
@L_8020C400:
    lbz r0, 0x8(r1)
@L_8020C404:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8020C430
    mr r3, r26
    rlwinm r4, r31, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C430
    bl fn_800F0308
    b @L_8020C3C0
@L_8020C430:
    lbz r0, 0x8(r1)
    cmplwi r0, 0x1
    bne @L_8020C4A8
    lfs f1, -20864(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    cmplwi r31, 0x0
    beq @L_8020C468
    mr r3, r26
    rlwinm r4, r31, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9B4
@L_8020C468:
    mr r3, r26
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9B4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C4A8
    lhz r4, -21032(r13)
    mr r3, r26
    li r5, 0x4
    bl fn_801DA9B4
@L_8020C4A8:
    cmplwi r27, 0x0
    beq @L_8020C50C
    mr r3, r26
    li r4, 0x5f
    li r5, 0x4
    bl fn_801DA9E8
    lbz r0, 0x8(r1)
    cmplwi r0, 0x1
    bne @L_8020C4E0
    lfs f1, -20864(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r0, 0x0
    stb r0, 0x8(r1)
@L_8020C4E0:
    mr r3, r27
    bl fn_80262490
@L_8020C4E8:
    mr r3, r26
    li r4, 0x5f
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C508
    bl fn_800F0308
    b @L_8020C4E8
@L_8020C508:
    bl fn_8026246C
@L_8020C50C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x11
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_8020C534
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
@L_8020C534:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C5D8
    mr r3, r29
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r29
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    mr r3, r26
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    lbz r0, 0x8(r1)
    cmplwi r0, 0x1
    bne @L_8020C5AC
    lfs f1, -20864(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r0, 0x0
    stb r0, 0x8(r1)
@L_8020C5AC:
    li r3, 0x766d
    bl fn_80262490
@L_8020C5B4:
    mr r3, r26
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C5D4
    bl fn_800F0308
    b @L_8020C5B4
@L_8020C5D4:
    bl fn_8026246C
@L_8020C5D8:
    lbz r0, 0x8(r1)
    cmplwi r0, 0x1
    bne @L_8020C5F0
    lfs f1, -20864(r2)
    li r3, 0x2
    bl fn_801C41C8
@L_8020C5F0:
    cmplwi r30, 0x0
    beq @L_8020C650
    cmplwi r31, 0x0
    beq @L_8020C610
    mr r3, r26
    rlwinm r4, r31, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
@L_8020C610:
    mr r3, r26
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C650
    lhz r4, -21032(r13)
    mr r3, r26
    li r5, 0x4
    bl fn_801DA8C4
@L_8020C650:
    cmplwi r27, 0x0
    beq @L_8020C668
    mr r3, r26
    li r4, 0x5f
    li r5, 0x4
    bl fn_801DA8C4
@L_8020C668:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C828
    mr r3, r26
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    b @L_8020C828
@L_8020C69C:
    li r3, 0x0
    bl fn_801EF7C4
    mr r3, r25
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r25
    li r4, 0x54
    li r5, 0x4
    bl fn_801DA9E8
@L_8020C6C0:
    mr r3, r25
    li r4, 0x54
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C6E0
    bl fn_800F0308
    b @L_8020C6C0
@L_8020C6E0:
    li r3, 0x0
    bl fn_801EF7C4
    mr r3, r26
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r26
    li r4, 0x55
    li r5, 0x4
    bl fn_801DA9E8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C754
    mr r3, r29
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r29
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    li r3, 0x766d
    bl fn_80262490
@L_8020C754:
    mr r3, r26
    li r4, 0x55
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C774
    bl fn_800F0308
    b @L_8020C754
@L_8020C774:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020C798
    bl fn_8026246C
@L_8020C798:
    li r3, 0x1
    bl fn_801EF7C4
    mr r3, r25
    li r4, 0x56
    li r5, 0x4
    bl fn_801DA9E8
@L_8020C7B0:
    mr r3, r25
    li r4, 0x56
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020C7D0
    bl fn_800F0308
    b @L_8020C7B0
@L_8020C7D0:
    mr r3, r25
    li r4, 0x54
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r26
    li r4, 0x55
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r25
    li r4, 0x56
    li r5, 0x4
    bl fn_801DA8C4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x11
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_8020C828
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
@L_8020C828:
    lmw r24, 0x60(r1)
    li r3, 0x1
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
