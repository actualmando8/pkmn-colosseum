    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r31, r3
    mr r21, r4
    li r3, 0x53
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80093F18
    lis r3, -32704
    addi r30, r3, -19584
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    beq @L_80093F18
    lha r0, 0x6(r21)
    li r3, -0x100
    lbz r4, 0x8b(r31)
    cmpwi r0, 0x1f7
    or r23, r4, r3
    beq @L_80093BBC
    bge @L_80093BB0
    cmpwi r0, 0x1f6
    bge @L_80093BE8
    b @L_80093F18
@L_80093BB0:
    cmpwi r0, 0x1291
    beq @L_80093D24
    b @L_80093F18
@L_80093BBC:
    lwz r4, 0x1c(r30)
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r21)
    mr r7, r23
    lha r6, 0x56(r21)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdd
    bl fn_800FBB34
    b @L_80093F18
@L_80093BE8:
    lbz r7, 0x1a(r30)
    lis r3, -32729
    addi r6, r3, -2588
    li r4, 0xc8
    lis r3, 0x81
    extsb r7, r7
    subi r5, r3, 0x1
    li r3, 0x78
    crxor 6, 6, 6
    bl fn_800FAEF8
    lis r3, -32704
    addi r5, r3, -19584
    lbz r0, 0x1(r5)
    cmplwi r0, 0x6
    bne @L_80093F18
    lbz r6, 0x1a(r30)
    lis r3, 0x38e4
    subi r0, r3, 0x71c7
    extsb r6, r6
    mulhw r0, r0, r6
    srawi r3, r0, 1
    rlwinm r4, r3, 1, 31, 31
    srawi r0, r0, 1
    add r3, r3, r4
    mulli r4, r3, 0x9
    rlwinm r3, r0, 1, 31, 31
    add r0, r0, r3
    subf r3, r4, r6
    rlwinm r3, r3, 2, 0, 29
    add r3, r5, r3
    add r3, r3, r0
    lbz r24, 0x20(r3)
    extsb r24, r24
    cmpwi r24, 0x0
    bge @L_80093C7C
    cmplwi r24, 0x20
    bge @L_80093F18
@L_80093C7C:
    lis r3, -32729
    lis r4, 0x81
    addi r6, r3, -2588
    mr r7, r24
    subi r5, r4, 0x1
    li r3, 0x78
    li r4, 0xc8
    crxor 6, 6, 6
    bl fn_800FAEF8
    mulli r4, r24, 0xc
    lis r3, -32721
    addi r0, r3, -4796
    add r3, r0, r4
    addi r3, r3, 0x8
    lwz r3, 0x0(r3)
    cmpwi r3, -0x1
    beq @L_80093CFC
    blt @L_80093D04
    cmpwi r3, 0x7
    bge @L_80093D04
    bl fn_80265F14
    rlwinm r3, r3, 0, 24, 31
    cmplwi r3, 0x0
    beq @L_80093CF4
    subi r0, r3, 0x1
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -4412
    lwzx r22, r3, r0
    b @L_80093D04
@L_80093CF4:
    li r22, 0x0
    b @L_80093D04
@L_80093CFC:
    add r3, r0, r4
    lwz r22, 0x4(r3)
@L_80093D04:
    lha r5, 0x54(r21)
    mr r7, r23
    lha r6, 0x56(r21)
    mr r8, r22
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_80093F18
@L_80093D24:
    mr r26, r30
    li r22, 0x0
    li r27, 0x0
@L_80093D30:
    mr r29, r27
    mr r28, r26
    li r24, 0x0
@L_80093D3C:
    lbz r0, 0x20(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80093DC8
    mulli r4, r0, 0xc
    lis r3, -32721
    addi r0, r3, -4796
    add r25, r0, r4
    lhz r3, 0x2(r25)
    bl fn_8005D934
    lbz r0, 0x1a(r30)
    extsb r0, r0
    cmpw r29, r0
    beq @L_80093DC8
    lha r5, 0x54(r21)
    lis r4, 0x38e4
    lha r0, 0x56(r21)
    subi r10, r4, 0x71c7
    mullw r4, r5, r24
    lha r5, 0x6(r3)
    lha r6, 0x8(r3)
    mr r7, r23
    lhz r9, 0x0(r25)
    mr r8, r31
    mulhw r3, r10, r4
    li r10, 0x0
    srawi r3, r3, 1
    mullw r0, r0, r22
    rlwinm r4, r3, 1, 31, 31
    add r3, r3, r4
    srawi r0, r0, 2
    extsh r3, r3
    addze r0, r0
    extsh r4, r0
    bl fn_80104160
@L_80093DC8:
    addi r29, r29, 0x1
    addi r28, r28, 0x4
    addi r24, r24, 0x1
    cmpwi r24, 0x9
    blt @L_80093D3C
    addi r27, r27, 0x9
    addi r26, r26, 0x1
    addi r22, r22, 0x1
    cmpwi r22, 0x4
    blt @L_80093D30
    lbz r0, 0x1a(r30)
    extsb r6, r0
    cmpwi r6, 0x0
    blt @L_80093F18
    lis r4, 0x38e4
    lis r3, -32704
    subi r0, r4, 0x71c7
    mulhw r0, r0, r6
    addi r3, r3, -19584
    srawi r4, r0, 1
    rlwinm r5, r4, 1, 31, 31
    srawi r0, r0, 1
    add r4, r4, r5
    mulli r5, r4, 0x9
    rlwinm r4, r0, 1, 31, 31
    add r24, r0, r4
    subf r22, r5, r6
    rlwinm r0, r22, 2, 0, 29
    add r0, r3, r0
    add r3, r0, r24
    lbz r0, 0x20(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80093F18
    mulli r25, r0, 0xc
    lis r3, -32721
    addi r0, r3, -4796
    add r3, r0, r25
    lhz r3, 0x2(r3)
    bl fn_8005D934
    lha r6, 0x54(r21)
    lis r5, 0x38e4
    lha r0, 0x56(r21)
    lis r4, -32721
    mullw r6, r6, r22
    subi r5, r5, 0x71c7
    addi r21, r4, -4796
    mr r30, r3
    lhzx r3, r21, r25
    mulhw r4, r5, r6
    srawi r4, r4, 1
    mullw r0, r0, r24
    rlwinm r5, r4, 1, 31, 31
    add r4, r4, r5
    srawi r0, r0, 2
    extsh r22, r4
    addze r0, r0
    extsh r24, r0
    bl fn_8005D858
    lha r5, 0xc(r3)
    mr r7, r23
    lha r4, 0x6(r30)
    mr r8, r31
    lha r6, 0xe(r3)
    li r10, 0x0
    lha r0, 0x8(r30)
    subf r3, r4, r5
    extsh r11, r3
    lhzx r9, r21, r25
    subf r0, r0, r6
    rlwinm r4, r11, 1, 31, 31
    extsh r3, r0
    add r4, r4, r11
    rlwinm r0, r3, 1, 31, 31
    srawi r4, r4, 1
    add r0, r0, r3
    srawi r0, r0, 1
    subf r3, r4, r22
    subf r0, r0, r24
    extsh r22, r3
    extsh r24, r0
    mr r3, r22
    mr r4, r24
    bl fn_80104160
@L_80093F18:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
