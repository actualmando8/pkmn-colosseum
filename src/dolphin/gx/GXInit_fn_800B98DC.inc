    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    lis r0, 0x4330
    stwu r1, -0x78(r1)
    stfd f31, 0x70(r1)
    stfd f30, 0x68(r1)
    stfd f29, 0x60(r1)
    stfd f28, 0x58(r1)
    stmw r25, 0x3c(r1)
    rlwinm r28, r4, 0, 16, 31
    rlwinm r31, r3, 0, 16, 31
    stw r28, 0x34(r1)
    mr r27, r3
    mr r25, r4
    stw r31, 0x2c(r1)
    stw r0, 0x30(r1)
    stw r0, 0x28(r1)
    lfd f3, -29584(r2)
    lfd f2, 0x30(r1)
    lfd f1, 0x28(r1)
    fsubs f2, f2, f3
    lfs f0, -29592(r2)
    fsubs f1, f1, f3
    fdivs f2, f2, f1
    fdivs f1, f0, f2
    fmr f28, f2
    bl fn_800C46B0
    subi r0, r31, 0x1
    rlwinm r4, r3, 0, 23, 31
    rlwinm r30, r0, 8, 0, 23
    divwu r3, r30, r4
    cmplwi r4, 0x80
    addi r3, r3, 0x1
    ble @L_800B9990
    cmplwi r4, 0x100
    bge @L_800B9990
    b @L_800B9974
@L_800B9970:
    rlwinm r4, r4, 31, 1, 31
@L_800B9974:
    rlwinm. r0, r4, 0, 31, 31
    beq @L_800B9970
    divwu r0, r31, r4
    mullw r0, r0, r4
    subf. r0, r0, r31
    bne @L_800B9990
    addi r3, r3, 0x1
@L_800B9990:
    cmplwi r3, 0x400
    ble @L_800B999C
    li r3, 0x400
@L_800B999C:
    lfd f31, -29584(r2)
    mr r0, r3
    lfs f30, -29592(r2)
    rlwinm r29, r25, 0, 16, 31
    rlwinm r26, r27, 0, 16, 31
    lis r25, 0x4330
    b @L_800B9A3C
@L_800B99B8:
    subi r28, r28, 0x1
    stw r26, 0x34(r1)
    stw r28, 0x2c(r1)
    stw r25, 0x28(r1)
    stw r25, 0x30(r1)
    lfd f1, 0x28(r1)
    lfd f0, 0x30(r1)
    fsubs f1, f1, f31
    fsubs f0, f0, f31
    fdivs f0, f1, f0
    fdivs f1, f30, f0
    fmr f28, f0
    bl fn_800C46B0
    rlwinm r4, r3, 0, 23, 31
    divwu r3, r30, r4
    cmplwi r4, 0x80
    addi r3, r3, 0x1
    ble @L_800B9A2C
    cmplwi r4, 0x100
    bge @L_800B9A2C
    b @L_800B9A10
@L_800B9A0C:
    rlwinm r4, r4, 31, 1, 31
@L_800B9A10:
    rlwinm. r0, r4, 0, 31, 31
    beq @L_800B9A0C
    divwu r0, r31, r4
    mullw r0, r0, r4
    subf. r0, r0, r31
    bne @L_800B9A2C
    addi r3, r3, 0x1
@L_800B9A2C:
    cmplwi r3, 0x400
    ble @L_800B9A38
    li r3, 0x400
@L_800B9A38:
    mr r0, r3
@L_800B9A3C:
    cmplw r0, r29
    bgt @L_800B99B8
    fmr f29, f28
    lfd f30, -29584(r2)
    lfs f31, -29592(r2)
    rlwinm r27, r27, 0, 16, 31
    lis r26, 0x4330
    b @L_800B9AE4
@L_800B9A5C:
    addi r28, r28, 0x1
    stw r27, 0x34(r1)
    fmr f29, f28
    stw r28, 0x2c(r1)
    stw r26, 0x28(r1)
    stw r26, 0x30(r1)
    lfd f1, 0x28(r1)
    lfd f0, 0x30(r1)
    fsubs f1, f1, f30
    fsubs f0, f0, f30
    fdivs f0, f1, f0
    fdivs f1, f31, f0
    fmr f28, f0
    bl fn_800C46B0
    rlwinm r4, r3, 0, 23, 31
    divwu r3, r30, r4
    cmplwi r4, 0x80
    addi r3, r3, 0x1
    ble @L_800B9AD4
    cmplwi r4, 0x100
    bge @L_800B9AD4
    b @L_800B9AB8
@L_800B9AB4:
    rlwinm r4, r4, 31, 1, 31
@L_800B9AB8:
    rlwinm. r0, r4, 0, 31, 31
    beq @L_800B9AB4
    divwu r0, r31, r4
    mullw r0, r0, r4
    subf. r0, r0, r31
    bne @L_800B9AD4
    addi r3, r3, 0x1
@L_800B9AD4:
    cmplwi r3, 0x400
    ble @L_800B9AE0
    li r3, 0x400
@L_800B9AE0:
    mr r0, r3
@L_800B9AE4:
    cmplw r0, r29
    blt @L_800B9A5C
    lmw r25, 0x3c(r1)
    fmr f1, f29
    lwz r0, 0x7c(r1)
    lfd f31, 0x70(r1)
    lfd f30, 0x68(r1)
    lfd f29, 0x60(r1)
    lfd f28, 0x58(r1)
    addi r1, r1, 0x78
    mtlr r0
    blr
