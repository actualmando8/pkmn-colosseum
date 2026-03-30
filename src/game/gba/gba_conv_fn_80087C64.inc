    nofralloc
    stwu r1, -0x1c0(r1)
    mflr r0
    stw r0, 0x1c4(r1)
    stmw r18, 0x188(r1)
    mr r25, r3
    lis r4, -32729
    li r3, 0x1
    addi r30, r4, -2936
    bl fn_801054B8
    addi r29, r1, 0x88
    li r28, 0x1
    li r27, 0x1
    li r26, 0x0
    li r0, 0x0
    stw r0, 0x88(r1)
    stw r0, 0x8c(r1)
    stw r0, 0x90(r1)
    stw r0, 0x94(r1)
    stw r0, 0x98(r1)
    stw r0, 0x9c(r1)
    stw r0, 0xa0(r1)
    stw r0, 0xa4(r1)
    stw r0, 0xa8(r1)
    b @L_800883F8
@L_80087CC4:
    li r3, 0x1
    bl fn_801054B8
    mr r31, r3
    li r6, 0x0
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80088124
    cmpwi r28, 0x0
    blt @L_80087D04
    cmpwi r28, 0x3
    bge @L_80087D04
    cmpwi r27, 0x0
    blt @L_80087D04
    cmpwi r27, 0x3
    blt @L_80087D0C
@L_80087D04:
    li r0, 0x0
    b @L_80087DD4
@L_80087D0C:
    mulli r3, r27, 0xc
    rlwinm r0, r28, 2, 0, 29
    addi r24, r1, 0x88
    add r20, r3, r0
    lwzx r0, r24, r20
    cmpwi r0, 0x0
    beq @L_80087D30
    li r0, 0x0
    b @L_80087DD4
@L_80087D30:
    li r0, 0x9
    addi r5, r1, 0xf0
    addi r4, r30, 0x10
    mtctr r0
@L_80087D40:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80087D40
    cmpwi r28, 0x0
    blt @L_80087DCC
    cmpwi r28, 0x3
    bge @L_80087DCC
    cmpwi r27, 0x0
    blt @L_80087DCC
    cmpwi r27, 0x3
    bge @L_80087DCC
    mulli r4, r27, 0x18
    rlwinm r0, r28, 3, 0, 28
    addi r3, r1, 0xf4
    add r4, r4, r0
    add r4, r3, r4
    lha r19, 0x4(r4)
    lwz r3, 0x0(r4)
    bl fn_800F92D4
    mr r21, r3
    cmplwi r21, 0x0
    beq @L_80087DCC
    beq @L_80087DCC
    extsh r4, r19
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r21
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r21
    bl fn_800EC9DC
    mr r3, r21
    bl fn_800EC990
@L_80087DCC:
    li r0, 0x1
    stwx r0, r24, r20
@L_80087DD4:
    cmpwi r0, 0x0
    beq @L_80088120
    rlwinm r0, r26, 3, 0, 28
    addi r4, r1, 0x70
    add r4, r4, r0
    addi r26, r26, 0x1
    stw r28, 0x0(r4)
    cmpwi r26, 0x0
    lwz r3, -29920(r2)
    lwz r0, -29916(r2)
    stw r27, 0x4(r4)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    blt @L_80087E64
    cmpwi r26, 0x4
    bge @L_80087E64
    rlwinm r0, r26, 1, 0, 30
    addi r4, r1, 0x18
    lis r3, 0x107e
    lhax r19, r4, r0
    addi r3, r3, 0x100b
    bl fn_800F92D4
    mr r20, r3
    cmplwi r20, 0x0
    beq @L_80087E64
    beq @L_80087E64
    extsh r4, r19
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r20
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r20
    bl fn_800EC9DC
    mr r3, r20
    bl fn_800EC990
@L_80087E64:
    cmpwi r26, 0x3
    blt @L_80088118
    lwz r6, 0x0(r30)
    li r7, 0x0
    lwz r5, 0x4(r30)
    mr r8, r7
    lwz r4, 0x8(r30)
    mr r12, r29
    lwz r3, 0xc(r30)
    addi r24, r1, 0x5c
    lhz r0, 0x10(r30)
    stw r6, 0x20(r1)
    stw r5, 0x24(r1)
    stw r4, 0x28(r1)
    stw r3, 0x2c(r1)
    sth r0, 0x30(r1)
    rlwinm r0, r0, 0, 16, 31
@L_80087EA8:
    stw r6, 0x34(r1)
    mr r10, r12
    mr r11, r24
    li r9, 0x0
    stw r5, 0x38(r1)
    stw r4, 0x3c(r1)
    stw r3, 0x40(r1)
    sth r0, 0x44(r1)
    li r20, 0x3
    mtctr r20
@L_80087ED0:
    lwz r19, 0x0(r10)
    cmpwi r19, 0x0
    beq @L_80087F6C
    lwz r23, 0x34(r1)
    cmpwi r9, 0x0
    lwz r22, 0x38(r1)
    lwz r21, 0x3c(r1)
    lwz r20, 0x40(r1)
    lhz r19, 0x44(r1)
    stw r23, 0x5c(r1)
    stw r22, 0x60(r1)
    stw r21, 0x64(r1)
    stw r20, 0x68(r1)
    sth r19, 0x6c(r1)
    blt @L_80087F24
    cmpwi r9, 0x3
    bge @L_80087F24
    cmpwi r8, 0x0
    blt @L_80087F24
    cmpwi r8, 0x3
    blt @L_80087F2C
@L_80087F24:
    li r19, 0x0
    b @L_80087F30
@L_80087F2C:
    lhz r19, 0x0(r11)
@L_80087F30:
    rlwinm r20, r19, 0, 16, 31
    lhz r19, 0x0(r25)
    cmplw r20, r19
    bne @L_80087F48
    addi r7, r7, 0x1
    b @L_80087F6C
@L_80087F48:
    lhz r19, 0x2(r25)
    cmplw r20, r19
    bne @L_80087F5C
    addi r7, r7, 0x1
    b @L_80087F6C
@L_80087F5C:
    lhz r19, 0x4(r25)
    cmplw r20, r19
    bne @L_80087F6C
    addi r7, r7, 0x1
@L_80087F6C:
    addi r10, r10, 0x4
    addi r11, r11, 0x2
    addi r9, r9, 0x1
    bdnz @L_80087ED0
    addi r12, r12, 0xc
    addi r24, r24, 0x6
    addi r8, r8, 0x1
    cmpwi r8, 0x3
    blt @L_80087EA8
    cmpwi r7, 0x3
    bge @L_800880F4
    li r3, 0x26
    bl fn_80166A28
    b @L_80087FA8
@L_80087FA4:
    bl fn_800F0308
@L_80087FA8:
    li r3, 0x26
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80087FA4
    mr r21, r29
    addi r20, r1, 0xac
    li r24, 0x0
@L_80087FC4:
    mr r23, r21
    mr r22, r20
    li r26, 0x0
@L_80087FD0:
    lwz r0, 0x0(r23)
    cmpwi r0, 0x0
    beq @L_8008806C
    li r0, 0x9
    addi r5, r1, 0xa8
    addi r4, r30, 0x10
    mtctr r0
@L_80087FEC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80087FEC
    cmpwi r26, 0x0
    blt @L_80088064
    cmpwi r26, 0x3
    bge @L_80088064
    cmpwi r24, 0x0
    blt @L_80088064
    cmpwi r24, 0x3
    bge @L_80088064
    lha r19, 0x6(r22)
    lwz r3, 0x0(r22)
    bl fn_800F92D4
    mr r18, r3
    cmplwi r18, 0x0
    beq @L_80088064
    beq @L_80088064
    extsh r4, r19
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r18
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r18
    bl fn_800EC9DC
    mr r3, r18
    bl fn_800EC990
@L_80088064:
    li r0, 0x0
    stw r0, 0x0(r23)
@L_8008806C:
    addi r23, r23, 0x4
    addi r22, r22, 0x8
    addi r26, r26, 0x1
    cmpwi r26, 0x3
    blt @L_80087FD0
    addi r21, r21, 0xc
    addi r20, r20, 0x18
    addi r24, r24, 0x1
    cmpwi r24, 0x3
    blt @L_80087FC4
    lwz r4, -29920(r2)
    lis r3, 0x107e
    lwz r0, -29916(r2)
    addi r3, r3, 0x100b
    stw r4, 0x10(r1)
    li r26, 0x0
    stw r0, 0x14(r1)
    lha r20, 0x10(r1)
    bl fn_800F92D4
    mr r19, r3
    cmplwi r19, 0x0
    beq @L_80088120
    beq @L_80088120
    extsh r4, r20
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r19
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r19
    bl fn_800EC9DC
    mr r3, r19
    bl fn_800EC990
    b @L_80088120
@L_800880F4:
    li r3, 0x4a1
    bl fn_80166A28
    b @L_80088104
@L_80088100:
    bl fn_800F0308
@L_80088104:
    li r3, 0x4a1
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80088100
    b @L_80088120
@L_80088118:
    li r3, 0x3c6
    bl fn_80166A28
@L_80088120:
    li r6, 0x1
@L_80088124:
    lhz r0, 0x4(r31)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80088294
    cmpwi r6, 0x0
    bne @L_80088294
    subi r26, r26, 0x1
    cmpwi r26, 0x0
    blt @L_80088400
    li r3, 0x3c7
    bl fn_80166A28
    rlwinm r0, r26, 3, 0, 28
    addi r20, r1, 0x70
    add r20, r20, r0
    li r0, 0x9
    addi r5, r1, 0x138
    addi r4, r30, 0x10
    lwz r7, 0x4(r20)
    lwz r6, 0x0(r20)
    mtctr r0
@L_80088174:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80088174
    cmpwi r6, 0x0
    blt @L_80088200
    cmpwi r6, 0x3
    bge @L_80088200
    cmpwi r7, 0x0
    blt @L_80088200
    cmpwi r7, 0x3
    bge @L_80088200
    mulli r4, r7, 0x18
    rlwinm r0, r6, 3, 0, 28
    addi r3, r1, 0x13c
    add r4, r4, r0
    add r4, r3, r4
    lha r21, 0x6(r4)
    lwz r3, 0x0(r4)
    bl fn_800F92D4
    mr r19, r3
    cmplwi r19, 0x0
    beq @L_80088200
    beq @L_80088200
    extsh r4, r21
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r19
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r19
    bl fn_800EC9DC
    mr r3, r19
    bl fn_800EC990
@L_80088200:
    lwz r3, -29920(r2)
    cmpwi r26, 0x0
    lwz r0, -29916(r2)
    stw r3, 0x8(r1)
    stw r0, 0xc(r1)
    blt @L_80088270
    cmpwi r26, 0x4
    bge @L_80088270
    rlwinm r0, r26, 1, 0, 30
    addi r4, r1, 0x8
    lis r3, 0x107e
    lhax r21, r4, r0
    addi r3, r3, 0x100b
    bl fn_800F92D4
    mr r19, r3
    cmplwi r19, 0x0
    beq @L_80088270
    beq @L_80088270
    extsh r4, r21
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r19
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r19
    bl fn_800EC9DC
    mr r3, r19
    bl fn_800EC990
@L_80088270:
    lwz r4, 0x4(r20)
    addi r3, r1, 0x88
    lwz r0, 0x0(r20)
    li r5, 0x0
    mulli r4, r4, 0xc
    li r6, 0x1
    rlwinm r0, r0, 2, 0, 29
    add r0, r4, r0
    stwx r5, r3, r0
@L_80088294:
    cmpwi r6, 0x0
    bne @L_800883F4
    lhz r4, 0x6(r31)
    mr r31, r28
    mr r24, r27
    li r3, 0x0
    rlwinm r0, r4, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_800882C8
    cmpwi r27, 0x0
    ble @L_800882C8
    subi r24, r27, 0x1
    li r3, 0x1
@L_800882C8:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800882E4
    cmpwi r24, 0x2
    bge @L_800882E4
    addi r24, r24, 0x1
    li r3, 0x1
@L_800882E4:
    rlwinm r0, r4, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80088300
    cmpwi r28, 0x0
    ble @L_80088300
    subi r31, r31, 0x1
    li r3, 0x1
@L_80088300:
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8008831C
    cmpwi r31, 0x2
    bge @L_8008831C
    addi r31, r31, 0x1
    li r3, 0x1
@L_8008831C:
    cmpwi r3, 0x0
    beq @L_800883F4
    lwz r6, 0x5c(r30)
    cmpwi r31, 0x0
    lwz r5, 0x60(r30)
    lwz r4, 0x64(r30)
    lwz r3, 0x68(r30)
    lhz r0, 0x6c(r30)
    stw r6, 0x48(r1)
    stw r5, 0x4c(r1)
    stw r4, 0x50(r1)
    stw r3, 0x54(r1)
    sth r0, 0x58(r1)
    blt @L_800883EC
    cmpwi r31, 0x3
    bge @L_800883EC
    cmpwi r24, 0x0
    blt @L_800883EC
    cmpwi r24, 0x3
    bge @L_800883EC
    mulli r5, r24, 0x6
    rlwinm r0, r31, 1, 0, 30
    lis r3, 0x107e
    addi r4, r1, 0x48
    add r0, r5, r0
    addi r3, r3, 0x1009
    lhax r20, r4, r0
    bl fn_800F92D4
    mr r18, r3
    cmplwi r18, 0x0
    beq @L_800883EC
    beq @L_800883C4
    extsh r4, r20
    bl fn_800ECCA8
    lfs f1, -29908(r2)
    mr r3, r18
    bl fn_800ECA78
    lfs f1, -29912(r2)
    mr r3, r18
    bl fn_800EC9DC
    mr r3, r18
    bl fn_800EC990
@L_800883C4:
    mr r3, r18
    li r4, 0x0
    bl fn_800ECB74
    b @L_800883D8
@L_800883D4:
    bl fn_800F0308
@L_800883D8:
    mr r3, r18
    bl fn_800EC960
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800883D4
@L_800883EC:
    mr r28, r31
    mr r27, r24
@L_800883F4:
    bl fn_800F0308
@L_800883F8:
    cmpwi r26, 0x3
    blt @L_80087CC4
@L_80088400:
    cmpwi r26, 0x0
    bge @L_80088410
    li r3, 0x1
    b @L_80088414
@L_80088410:
    li r3, 0x0
@L_80088414:
    lmw r18, 0x188(r1)
    lwz r0, 0x1c4(r1)
    mtlr r0
    addi r1, r1, 0x1c0
    blr
