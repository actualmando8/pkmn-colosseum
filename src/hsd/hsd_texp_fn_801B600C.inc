    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, -32729
    stw r0, 0x24(r1)
    rlwinm r0, r6, 3, 0, 28
    stw r31, 0x1c(r1)
    addi r31, r7, 21416
    stw r30, 0x18(r1)
    mr r30, r6
    stw r29, 0x14(r1)
    mr. r29, r5
    stw r28, 0x10(r1)
    mr r28, r3
    add r5, r28, r0
    rlwinm r3, r6, 3, 0, 28
    rlwinm r0, r6, 3, 0, 28
    lwz r6, 0x54(r5)
    lwz r5, 0x58(r5)
    add r3, r28, r3
    stw r6, 0x8(r1)
    stb r4, 0x55(r3)
    add r3, r28, r0
    stw r5, 0xc(r1)
    stw r29, 0x58(r3)
    bne @L_801B6078
    li r6, 0x0
    b @L_801B60A4
@L_801B6078:
    addis r0, r29, 0x1
    cmplwi r0, 0xffff
    bne @L_801B608C
    li r6, 0x2
    b @L_801B60A4
@L_801B608C:
    addis r0, r29, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B60A0
    li r6, 0x3
    b @L_801B60A4
@L_801B60A0:
    lwz r6, 0x0(r29)
@L_801B60A4:
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r5, r28, r3
    cmpwi r4, 0x7
    stb r6, 0x54(r5)
    add r3, r28, r0
    li r0, 0xff
    stb r0, 0x56(r3)
    beq @L_801B60D8
    blt @L_801B61D4
    cmpwi r4, 0x10
    bge @L_801B61D4
    b @L_801B610C
@L_801B60D8:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    add r4, r28, r0
    li r0, 0x0
    stb r0, 0x54(r4)
    rlwinm r0, r30, 3, 0, 28
    add r3, r28, r3
    li r4, 0x7
    stb r4, 0x56(r3)
    add r3, r28, r0
    li r0, 0x0
    stw r0, 0x58(r3)
    b @L_801B64C0
@L_801B610C:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r3, r30, 3, 0, 28
    add r5, r28, r0
    li r6, 0x0
    subi r0, r4, 0x9
    stw r6, 0x58(r5)
    cmplwi r0, 0x6
    add r3, r28, r3
    li r4, 0x6
    stb r4, 0x56(r3)
    bgt @L_801B6188
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -11452
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r4, 0x7
    b @L_801B618C
    li r4, 0x6
    b @L_801B618C
    li r4, 0x5
    b @L_801B618C
    li r4, 0x4
    b @L_801B618C
    li r4, 0x3
    b @L_801B618C
    li r4, 0x2
    b @L_801B618C
    li r4, 0x1
    b @L_801B618C
@L_801B6188:
    li r4, 0x0
@L_801B618C:
    lbz r0, 0x21(r28)
    cmplwi r0, 0xff
    bne @L_801B61A0
    stb r4, 0x21(r28)
    b @L_801B61C0
@L_801B61A0:
    lbz r3, 0x21(r28)
    rlwinm r0, r4, 0, 24, 31
    cmplw r3, r0
    beq @L_801B61C0
    addi r5, r31, 0x90
    addi r3, r2, -22576
    li r4, 0x30d
    bl fn_80196E10
@L_801B61C0:
    rlwinm r0, r30, 3, 0, 28
    li r4, 0x6
    add r3, r28, r0
    stb r4, 0x54(r3)
    b @L_801B64C0
@L_801B61D4:
    rlwinm r0, r30, 3, 0, 28
    add r3, r28, r0
    lbz r0, 0x54(r3)
    cmpwi r0, 0x2
    beq @L_801B6458
    bge @L_801B61FC
    cmpwi r0, 0x0
    beq @L_801B620C
    bge @L_801B6250
    b @L_801B64B0
@L_801B61FC:
    cmpwi r0, 0x4
    beq @L_801B6344
    bge @L_801B64B0
    b @L_801B6484
@L_801B620C:
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r28, r3
    li r4, 0x0
    stw r4, 0x58(r3)
    rlwinm r3, r30, 3, 0, 28
    add r4, r28, r0
    li r0, 0x0
    stb r0, 0x54(r4)
    rlwinm r0, r30, 3, 0, 28
    add r3, r28, r3
    li r4, 0x7
    stb r4, 0x55(r3)
    add r3, r28, r0
    li r0, 0x7
    stb r0, 0x56(r3)
    b @L_801B64C0
@L_801B6250:
    cmpwi r4, 0x5
    beq @L_801B6268
    addi r5, r31, 0xa4
    addi r3, r2, -22576
    li r4, 0x324
    bl fn_80196E10
@L_801B6268:
    cmpwi r30, 0x3
    li r3, 0x0
    beq @L_801B6280
    lbz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_801B6284
@L_801B6280:
    li r3, 0x1
@L_801B6284:
    cmpwi r3, 0x0
    bne @L_801B629C
    addi r5, r31, 0xb4
    addi r3, r2, -22576
    li r4, 0x325
    bl fn_80196E10
@L_801B629C:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    add r3, r28, r0
    lwz r5, 0x58(r3)
    add r3, r28, r4
    lbz r3, 0x55(r3)
    cmplwi r5, 0x0
    bne @L_801B62C4
    li r0, 0x0
    b @L_801B62F0
@L_801B62C4:
    addis r0, r5, 0x1
    cmplwi r0, 0xffff
    bne @L_801B62D8
    li r0, 0x2
    b @L_801B62F0
@L_801B62D8:
    addis r0, r5, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B62EC
    li r0, 0x3
    b @L_801B62F0
@L_801B62EC:
    lwz r0, 0x0(r5)
@L_801B62F0:
    cmpwi r0, 0x4
    beq @L_801B6334
    bge @L_801B64C0
    cmpwi r0, 0x1
    beq @L_801B6308
    b @L_801B64C0
@L_801B6308:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B6324
    lwz r3, 0x8(r5)
    addi r0, r3, 0x1
    stw r0, 0x8(r5)
    b @L_801B64C0
@L_801B6324:
    lwz r3, 0x14(r5)
    addi r0, r3, 0x1
    stw r0, 0x14(r5)
    b @L_801B64C0
@L_801B6334:
    lbz r3, 0x10(r5)
    addi r0, r3, 0x1
    stb r0, 0x10(r5)
    b @L_801B64C0
@L_801B6344:
    subi r4, r4, 0x5
    li r3, 0x1
    subf r0, r4, r3
    li r5, 0x1
    orc r3, r3, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_801B636C
    li r5, 0x0
@L_801B636C:
    cmpwi r5, 0x0
    bne @L_801B6384
    addi r5, r31, 0xd4
    addi r3, r2, -22576
    li r4, 0x329
    bl fn_80196E10
@L_801B6384:
    lbz r0, 0xc(r29)
    cmplwi r0, 0x6
    beq @L_801B63A0
    addi r5, r31, 0xf8
    addi r3, r2, -22576
    li r4, 0x32a
    bl fn_80196E10
@L_801B63A0:
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r28, r3
    li r4, 0x6
    stb r4, 0x55(r3)
    add r3, r28, r0
    rlwinm r0, r30, 3, 0, 28
    lwz r4, 0x58(r3)
    add r3, r28, r0
    lbz r3, 0x55(r3)
    cmplwi r4, 0x0
    bne @L_801B63D8
    li r0, 0x0
    b @L_801B6404
@L_801B63D8:
    addis r0, r4, 0x1
    cmplwi r0, 0xffff
    bne @L_801B63EC
    li r0, 0x2
    b @L_801B6404
@L_801B63EC:
    addis r0, r4, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6400
    li r0, 0x3
    b @L_801B6404
@L_801B6400:
    lwz r0, 0x0(r4)
@L_801B6404:
    cmpwi r0, 0x4
    beq @L_801B6448
    bge @L_801B64C0
    cmpwi r0, 0x1
    beq @L_801B641C
    b @L_801B64C0
@L_801B641C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B6438
    lwz r3, 0x8(r4)
    addi r0, r3, 0x1
    stw r0, 0x8(r4)
    b @L_801B64C0
@L_801B6438:
    lwz r3, 0x14(r4)
    addi r0, r3, 0x1
    stw r0, 0x14(r4)
    b @L_801B64C0
@L_801B6448:
    lbz r3, 0x10(r4)
    addi r0, r3, 0x1
    stb r0, 0x10(r4)
    b @L_801B64C0
@L_801B6458:
    cmpwi r4, 0x5
    beq @L_801B6470
    addi r5, r31, 0xa4
    addi r3, r2, -22576
    li r4, 0x32f
    bl fn_80196E10
@L_801B6470:
    rlwinm r0, r30, 3, 0, 28
    li r4, 0x4
    add r3, r28, r0
    stb r4, 0x56(r3)
    b @L_801B64C0
@L_801B6484:
    cmpwi r4, 0x5
    beq @L_801B649C
    addi r5, r31, 0xa4
    addi r3, r2, -22576
    li r4, 0x333
    bl fn_80196E10
@L_801B649C:
    rlwinm r0, r30, 3, 0, 28
    li r4, 0x5
    add r3, r28, r0
    stb r4, 0x56(r3)
    b @L_801B64C0
@L_801B64B0:
    addi r3, r2, -22576
    li r4, 0x337
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B64C0:
    lwz r3, 0xc(r1)
    lbz r4, 0x9(r1)
    bl fn_801B750C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
