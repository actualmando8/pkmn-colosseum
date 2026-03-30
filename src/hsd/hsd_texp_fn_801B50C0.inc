    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    ble @L_801B52EC
    lbz r0, 0x20(r31)
    cmplwi r0, 0xff
    beq @L_801B5164
    li r30, 0x0
    b @L_801B5158
@L_801B5104:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x4
    bne @L_801B5154
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801B5C94
    mr. r29, r3
    bge @L_801B5154
    cmpwi r29, 0x0
    bge @L_801B514C
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x49c
    bl fn_80196E10
@L_801B514C:
    mr r3, r29
    b @L_801B5830
@L_801B5154:
    addi r30, r30, 0x1
@L_801B5158:
    cmpwi r30, 0x4
    blt @L_801B5104
    b @L_801B52EC
@L_801B5164:
    lbz r0, 0xd(r31)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_801B51A8
    lbz r0, 0x35(r31)
    cmplwi r0, 0x7
    bne @L_801B51A8
    lbz r0, 0x3d(r31)
    cmplwi r0, 0x7
    bne @L_801B51A8
    lbz r0, 0xf(r31)
    cmplwi r0, 0x0
    bne @L_801B51A8
    lbz r0, 0x10(r31)
    cmplwi r0, 0x0
    bne @L_801B51A8
    li r3, 0x1
@L_801B51A8:
    cmpwi r3, 0x0
    beq @L_801B5210
    lbz r0, 0x4c(r31)
    cmplwi r0, 0x4
    bne @L_801B5210
    mr r3, r31
    mr r5, r28
    li r4, 0x3
    bl fn_801B5C94
    mr. r29, r3
    bge @L_801B52EC
    mr r3, r31
    mr r5, r28
    li r4, 0x3
    bl fn_801B5A00
    mr. r29, r3
    bge @L_801B52EC
    cmpwi r29, 0x0
    bge @L_801B5208
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4a8
    bl fn_80196E10
@L_801B5208:
    mr r3, r29
    b @L_801B5830
@L_801B5210:
    li r30, 0x0
    b @L_801B5284
@L_801B5218:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x4
    bne @L_801B5280
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801B5A00
    mr. r29, r3
    bge @L_801B5280
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801B5C94
    mr. r29, r3
    bge @L_801B5280
    cmpwi r29, 0x0
    bge @L_801B5278
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4b5
    bl fn_80196E10
@L_801B5278:
    mr r3, r29
    b @L_801B5830
@L_801B5280:
    addi r30, r30, 0x1
@L_801B5284:
    cmpwi r30, 0x4
    blt @L_801B5218
    b @L_801B52E4
@L_801B5290:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x4
    bne @L_801B52E0
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801B5C94
    mr. r29, r3
    bge @L_801B52E0
    cmpwi r29, 0x0
    bge @L_801B52D8
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4be
    bl fn_80196E10
@L_801B52D8:
    mr r3, r29
    b @L_801B5830
@L_801B52E0:
    addi r30, r30, 0x1
@L_801B52E4:
    cmpwi r30, 0x4
    blt @L_801B5290
@L_801B52EC:
    lwz r0, 0x14(r31)
    cmpwi r0, 0x0
    ble @L_801B582C
    lbz r0, 0x21(r31)
    cmplwi r0, 0xff
    beq @L_801B5444
    li r30, 0x0
    b @L_801B5438
@L_801B530C:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x4
    bne @L_801B5434
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lwz r9, 0x58(r3)
    lbz r0, 0xe(r9)
    cmplwi r0, 0xff
    beq @L_801B5388
    lbz r0, 0xe(r9)
    cmplwi r0, 0x4
    bge @L_801B534C
    li r6, -0x1
    b @L_801B5408
@L_801B534C:
    rlwinm r0, r30, 3, 0, 28
    lis r3, -32713
    add r4, r31, r0
    li r6, 0x0
    li r0, 0x5
    addi r5, r3, -11656
    stb r0, 0x54(r4)
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r4, 0xe(r9)
    rlwinm r0, r4, 2, 0, 29
    add r4, r5, r0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B5408
@L_801B5388:
    li r8, 0x4
    b @L_801B53FC
@L_801B5390:
    rlwinm r0, r8, 1, 0, 30
    add r3, r28, r0
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_801B53F8
    rlwinm r0, r8, 1, 0, 30
    rlwinm r4, r30, 3, 0, 28
    add r3, r28, r0
    li r5, 0x3
    li r0, 0x1
    add r6, r31, r4
    stb r0, 0xd(r3)
    lis r3, -32713
    li r7, 0x5
    rlwinm r4, r8, 2, 0, 29
    stb r8, 0xe(r9)
    rlwinm r0, r30, 3, 0, 28
    stb r5, 0xf(r9)
    addi r5, r3, -11656
    add r4, r5, r4
    add r3, r31, r0
    stb r7, 0x54(r6)
    li r6, 0x0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B5408
@L_801B53F8:
    addi r8, r8, 0x1
@L_801B53FC:
    cmpwi r8, 0x8
    blt @L_801B5390
    li r6, -0x1
@L_801B5408:
    mr. r29, r6
    bge @L_801B5434
    cmpwi r29, 0x0
    bge @L_801B542C
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4ce
    bl fn_80196E10
@L_801B542C:
    mr r3, r29
    b @L_801B5830
@L_801B5434:
    addi r30, r30, 0x1
@L_801B5438:
    cmpwi r30, 0x4
    blt @L_801B530C
    b @L_801B582C
@L_801B5444:
    lbz r0, 0x19(r31)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_801B5488
    lbz r0, 0x55(r31)
    cmplwi r0, 0x7
    bne @L_801B5488
    lbz r0, 0x5d(r31)
    cmplwi r0, 0x7
    bne @L_801B5488
    lbz r0, 0x1b(r31)
    cmplwi r0, 0x0
    bne @L_801B5488
    lbz r0, 0x1c(r31)
    cmplwi r0, 0x0
    bne @L_801B5488
    li r3, 0x1
@L_801B5488:
    cmpwi r3, 0x0
    beq @L_801B55A0
    lbz r0, 0x6c(r31)
    cmplwi r0, 0x4
    bne @L_801B55A0
    lwz r7, 0x70(r31)
    lbz r0, 0xe(r7)
    cmplwi r0, 0xff
    beq @L_801B54EC
    lbz r0, 0xe(r7)
    cmplwi r0, 0x4
    bge @L_801B54C0
    li r6, -0x1
    b @L_801B555C
@L_801B54C0:
    li r0, 0x5
    lis r3, -32713
    stb r0, 0x6c(r31)
    addi r3, r3, -11656
    li r6, 0x0
    lbz r0, 0xe(r7)
    rlwinm r0, r0, 2, 0, 29
    add r3, r3, r0
    lwz r0, -0x10(r3)
    stb r0, 0x6e(r31)
    b @L_801B555C
@L_801B54EC:
    li r6, 0x4
    b @L_801B5550
@L_801B54F4:
    rlwinm r0, r6, 1, 0, 30
    add r3, r28, r0
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_801B554C
    rlwinm r0, r6, 1, 0, 30
    lis r3, -32713
    add r4, r28, r0
    li r5, 0x3
    li r0, 0x1
    addi r3, r3, -11656
    stb r0, 0xd(r4)
    rlwinm r0, r6, 2, 0, 29
    li r4, 0x5
    stb r6, 0xe(r7)
    add r3, r3, r0
    li r6, 0x0
    stb r5, 0xf(r7)
    stb r4, 0x6c(r31)
    lwz r0, -0x10(r3)
    stb r0, 0x6e(r31)
    b @L_801B555C
@L_801B554C:
    addi r6, r6, 0x1
@L_801B5550:
    cmpwi r6, 0x8
    blt @L_801B54F4
    li r6, -0x1
@L_801B555C:
    mr. r29, r6
    bge @L_801B582C
    mr r3, r31
    mr r5, r28
    li r4, 0x3
    bl fn_801B5850
    mr. r29, r3
    bge @L_801B582C
    cmpwi r29, 0x0
    bge @L_801B5598
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4da
    bl fn_80196E10
@L_801B5598:
    mr r3, r29
    b @L_801B5830
@L_801B55A0:
    li r30, 0x0
    b @L_801B56EC
@L_801B55A8:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x4
    bne @L_801B56E8
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801B5850
    mr. r29, r3
    bge @L_801B56E8
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lwz r9, 0x58(r3)
    lbz r0, 0xe(r9)
    cmplwi r0, 0xff
    beq @L_801B563C
    lbz r0, 0xe(r9)
    cmplwi r0, 0x4
    bge @L_801B5600
    li r6, -0x1
    b @L_801B56BC
@L_801B5600:
    rlwinm r0, r30, 3, 0, 28
    lis r3, -32713
    add r4, r31, r0
    li r6, 0x0
    li r0, 0x5
    addi r5, r3, -11656
    stb r0, 0x54(r4)
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r4, 0xe(r9)
    rlwinm r0, r4, 2, 0, 29
    add r4, r5, r0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B56BC
@L_801B563C:
    li r8, 0x4
    b @L_801B56B0
@L_801B5644:
    rlwinm r0, r8, 1, 0, 30
    add r3, r28, r0
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_801B56AC
    rlwinm r0, r8, 1, 0, 30
    rlwinm r4, r30, 3, 0, 28
    add r3, r28, r0
    li r5, 0x3
    li r0, 0x1
    add r6, r31, r4
    stb r0, 0xd(r3)
    lis r3, -32713
    li r7, 0x5
    rlwinm r4, r8, 2, 0, 29
    stb r8, 0xe(r9)
    rlwinm r0, r30, 3, 0, 28
    stb r5, 0xf(r9)
    addi r5, r3, -11656
    add r4, r5, r4
    add r3, r31, r0
    stb r7, 0x54(r6)
    li r6, 0x0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B56BC
@L_801B56AC:
    addi r8, r8, 0x1
@L_801B56B0:
    cmpwi r8, 0x8
    blt @L_801B5644
    li r6, -0x1
@L_801B56BC:
    mr. r29, r6
    bge @L_801B56E8
    cmpwi r29, 0x0
    bge @L_801B56E0
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4e7
    bl fn_80196E10
@L_801B56E0:
    mr r3, r29
    b @L_801B5830
@L_801B56E8:
    addi r30, r30, 0x1
@L_801B56EC:
    cmpwi r30, 0x4
    blt @L_801B55A8
    b @L_801B5824
@L_801B56F8:
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x4
    bne @L_801B5820
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lwz r9, 0x58(r3)
    lbz r0, 0xe(r9)
    cmplwi r0, 0xff
    beq @L_801B5774
    lbz r0, 0xe(r9)
    cmplwi r0, 0x4
    bge @L_801B5738
    li r6, -0x1
    b @L_801B57F4
@L_801B5738:
    rlwinm r0, r30, 3, 0, 28
    lis r3, -32713
    add r4, r31, r0
    li r6, 0x0
    li r0, 0x5
    addi r5, r3, -11656
    stb r0, 0x54(r4)
    rlwinm r0, r30, 3, 0, 28
    add r3, r31, r0
    lbz r4, 0xe(r9)
    rlwinm r0, r4, 2, 0, 29
    add r4, r5, r0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B57F4
@L_801B5774:
    li r8, 0x4
    b @L_801B57E8
@L_801B577C:
    rlwinm r0, r8, 1, 0, 30
    add r3, r28, r0
    lbz r0, 0xd(r3)
    cmplwi r0, 0x0
    bne @L_801B57E4
    rlwinm r0, r8, 1, 0, 30
    rlwinm r4, r30, 3, 0, 28
    add r3, r28, r0
    li r5, 0x3
    li r0, 0x1
    add r6, r31, r4
    stb r0, 0xd(r3)
    lis r3, -32713
    li r7, 0x5
    rlwinm r4, r8, 2, 0, 29
    stb r8, 0xe(r9)
    rlwinm r0, r30, 3, 0, 28
    stb r5, 0xf(r9)
    addi r5, r3, -11656
    add r4, r5, r4
    add r3, r31, r0
    stb r7, 0x54(r6)
    li r6, 0x0
    lwz r0, -0x10(r4)
    stb r0, 0x56(r3)
    b @L_801B57F4
@L_801B57E4:
    addi r8, r8, 0x1
@L_801B57E8:
    cmpwi r8, 0x8
    blt @L_801B577C
    li r6, -0x1
@L_801B57F4:
    mr. r29, r6
    bge @L_801B5820
    cmpwi r29, 0x0
    bge @L_801B5818
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21428
    li r4, 0x4f0
    bl fn_80196E10
@L_801B5818:
    mr r3, r29
    b @L_801B5830
@L_801B5820:
    addi r30, r30, 0x1
@L_801B5824:
    cmpwi r30, 0x4
    blt @L_801B56F8
@L_801B582C:
    li r3, 0x0
@L_801B5830:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
