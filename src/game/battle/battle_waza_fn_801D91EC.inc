    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r26, 0x38(r1)
    mr r27, r3
    lwz r0, 0x28(r3)
    lwz r3, 0xb0(r3)
    cmplwi r0, 0x7
    lwz r29, 0xa4(r27)
    lwz r30, 0x3c(r3)
    bgt @L_801D9268
    lis r4, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, 9952
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r31, 0x1
    b @L_801D926C
    li r31, 0x2
    b @L_801D926C
    li r31, 0x3
    b @L_801D926C
    li r31, 0x4
    b @L_801D926C
    li r31, 0x5
    b @L_801D926C
    li r31, 0x6
    b @L_801D926C
    li r31, 0x7
    b @L_801D926C
@L_801D9268:
    li r31, 0x0
@L_801D926C:
    cmplwi r29, 0x0
    lwz r28, 0x24(r30)
    bne @L_801D9280
    li r3, 0x0
    b @L_801D97DC
@L_801D9280:
    lwz r0, 0x1c(r27)
    mr r26, r28
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D92D4
    lwz r4, 0x8(r27)
    cmpwi r4, 0x0
    ble @L_801D92D4
    bl fn_801DCDA8
    lwz r4, 0x70(r3)
    lwz r0, 0x70(r27)
    cmpw r4, r0
    bgt @L_801D92D4
    lwz r0, 0x4(r3)
    cmpwi r0, 0x2
    bne @L_801D92D4
    lwz r3, 0xa4(r3)
    cmplwi r3, 0x0
    beq @L_801D92D4
    mr r26, r3
    bl fn_800EC0E8
@L_801D92D4:
    mr r3, r26
    addi r4, r1, 0x14
    bl fn_800E3D98
    mr r3, r26
    addi r4, r1, 0x20
    bl fn_800E3D6C
    lwz r3, 0xb0(r27)
    addi r4, r1, 0x2c
    lwz r5, 0x10(r30)
    bl fn_801D9950
    li r0, 0x0
    stw r0, 0xa0(r27)
    lwz r3, 0x1c(r27)
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801D9350
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    bne @L_801D9640
    addi r3, r1, 0x2c
    bl fn_801C3B80
    lwz r0, 0x1c(r27)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x10
    bne @L_801D9340
    lfs f0, -21332(r2)
    stfs f0, 0x30(r1)
@L_801D9340:
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E407C
    b @L_801D9640
@L_801D9350:
    lwz r0, 0x94(r27)
    cmpwi r0, 0x0
    beq @L_801D9534
    lwz r4, 0x98(r27)
    mr r3, r29
    bl fn_800EE150
    mr r0, r3
    mr r3, r29
    mr r26, r0
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r29
    addi r4, r1, 0x20
    bl fn_800E4170
    cmpwi r31, 0x7
    beq @L_801D93B4
    cmpwi r31, 0x3
    beq @L_801D93B4
    cmpwi r31, 0x5
    beq @L_801D93B4
    cmpwi r31, 0x6
    beq @L_801D93B4
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E407C
@L_801D93B4:
    lwz r0, 0x9c(r27)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x10
    bne @L_801D945C
    cmplwi r26, 0x0
    beq @L_801D9450
    cmpwi r31, 0x0
    beq @L_801D9450
    mr r3, r28
    bl fn_800E3C5C
    mr r5, r3
    lfs f1, -21336(r2)
    addi r3, r1, 0x8
    addi r4, r5, 0x10
    addi r5, r5, 0x1c
    bl fn_800E090C
    addi r3, r1, 0x14
    addi r5, r1, 0x8
    mr r4, r3
    bl fn_800E019C
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_800E43A4
    addi r3, r1, 0x8
    mr r4, r3
    bl fn_800E00E0
    mr r3, r28
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    bl fn_800E732C
    mr r3, r28
    mr r4, r26
    mr r5, r31
    li r6, 0x0
    li r7, 0x1
    bl fn_800E4AC0
    li r0, 0x1
    stw r0, 0xa0(r27)
@L_801D9450:
    mr r3, r26
    bl fn_800EE828
    b @L_801D9640
@L_801D945C:
    cmplwi r26, 0x0
    beq @L_801D948C
    cmpwi r31, 0x0
    beq @L_801D948C
    mr r3, r28
    mr r4, r26
    mr r5, r31
    li r6, 0x0
    li r7, 0x1
    bl fn_800E4AC0
    li r0, 0x1
    stw r0, 0xa0(r27)
@L_801D948C:
    mr r3, r26
    bl fn_800EE828
    cmplwi r31, 0x7
    bgt @L_801D950C
    lis r3, -32713
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 9920
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_800E4170
    b @L_801D9640
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_800E43A4
    b @L_801D9640
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_800E4170
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_800E43A4
    b @L_801D9640
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_800E4170
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_800E43A4
    b @L_801D9640
@L_801D950C:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9520
    li r3, 0x1
    bl fn_801D744C
@L_801D9520:
    lis r3, -32728
    addi r3, r3, -26988
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D9640
@L_801D9534:
    cmpwi r31, 0x0
    beq @L_801D9640
    mr r3, r27
    bl fn_801D97F0
    mr. r26, r3
    beq @L_801D9640
    lwz r0, 0x1c(r27)
    mr r3, r29
    mr r4, r26
    mr r5, r31
    rlwinm r6, r0, 31, 31, 31
    li r7, 0x1
    bl fn_800E4AC0
    li r0, 0x1
    cmplwi r31, 0x7
    stw r0, 0xa0(r27)
    bgt @L_801D9614
    lis r3, -32713
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 9888
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E407C
    b @L_801D9638
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E407C
    mr r3, r29
    addi r4, r1, 0x20
    bl fn_800E4170
    b @L_801D9638
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_800E43A4
    b @L_801D9638
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E407C
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_800E43A4
    b @L_801D9638
    mr r3, r29
    addi r4, r1, 0x20
    bl fn_800E4170
    b @L_801D9638
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r29
    addi r4, r1, 0x20
    bl fn_800E4170
    b @L_801D9638
@L_801D9614:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9628
    li r3, 0x1
    bl fn_801D744C
@L_801D9628:
    lis r3, -32728
    addi r3, r3, -26932
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D9638:
    mr r3, r26
    bl fn_800EE828
@L_801D9640:
    mr r3, r29
    li r4, 0x1
    bl fn_800E4014
    lwz r0, 0x9c(r27)
    mr r3, r29
    rlwinm r4, r0, 0, 31, 31
    bl fn_800E3CC8
    lwz r0, 0x9c(r27)
    mr r3, r29
    rlwinm r4, r0, 30, 31, 31
    bl fn_800EC160
    lwz r0, 0x88(r27)
    cmpwi r0, 0x0
    blt @L_801D96E0
    mr r3, r29
    bl fn_800EC1BC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D96C4
    lwz r4, 0x88(r27)
    mr r3, r29
    bl fn_800ECCA8
    lwz r4, 0x84(r27)
    mr r3, r29
    bl fn_800ECB74
    lfs f1, -21336(r2)
    mr r3, r29
    bl fn_800EC9DC
    lfs f1, -21328(r2)
    mr r3, r29
    bl fn_800ECA78
    mr r3, r29
    bl fn_800EC990
    b @L_801D96E0
@L_801D96C4:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D96D8
    li r3, 0x200
    bl fn_801D744C
@L_801D96D8:
    li r0, -0x1
    stw r0, 0x88(r27)
@L_801D96E0:
    lwz r0, 0x88(r27)
    lwz r3, 0x90(r27)
    cmpw r0, r3
    beq @L_801D9760
    cmpwi r3, 0x0
    blt @L_801D9760
    mr r3, r29
    bl fn_800EC1B0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D9744
    lwz r4, 0x90(r27)
    mr r3, r29
    bl fn_800EC35C
    lwz r4, 0x8c(r27)
    mr r3, r29
    bl fn_800EC208
    lfs f1, -21336(r2)
    mr r3, r29
    bl fn_800EC308
    lfs f1, -21328(r2)
    mr r3, r29
    bl fn_800EC2A4
    mr r3, r29
    bl fn_800EC1E4
    b @L_801D9760
@L_801D9744:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9758
    li r3, 0x400
    bl fn_801D744C
@L_801D9758:
    li r0, -0x1
    stw r0, 0x90(r27)
@L_801D9760:
    lwz r0, 0x9c(r27)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x20
    bne @L_801D97D8
    bl fn_801DB848
    cmplwi r3, 0x0
    beq @L_801D97D8
    bl fn_801DB850
    cmpwi r3, 0x0
    beq @L_801D97D8
    mr r3, r29
    li r4, 0x1
    bl fn_800E9108
    bl fn_801DB848
    mr r4, r3
    mr r3, r29
    bl fn_800E8FE8
    bl fn_801DB858
    mr r27, r3
    bl fn_801DB850
    mr r4, r3
    mr r3, r29
    mr r5, r27
    bl fn_800E900C
    mr r3, r29
    li r4, 0x1
    bl fn_800E3C94
    mr r3, r29
    li r4, 0x1
    bl fn_800E3B44
@L_801D97D8:
    li r3, 0x1
@L_801D97DC:
    lmw r26, 0x38(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
