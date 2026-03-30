    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r30, r3
    addi r31, r30, 0x1
    li r4, 0x2
    mr r3, r31
    bl fn_8008ABE4
    mr r3, r30
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_8007173C
    mr r27, r3
    b @L_80071984
@L_8007173C:
    li r0, 0x44
    mr r3, r30
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0xa
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80071764
    li r27, 0xb
    b @L_80071858
@L_80071764:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r26, r0, 0x64
    bl OSGetTick
    lis r5, -32709
    lis r4, -32709
    rlwinm r6, r30, 3, 0, 28
    mr r29, r3
    addi r0, r5, 28184
    rlwinm r28, r30, 2, 0, 29
    add r24, r0, r6
    addi r27, r4, 28168
    addi r25, r24, 0x4
@L_800717AC:
    bl OSGetTick
    subf r0, r29, r3
    cmplw r0, r26
    ble @L_800717C4
    li r3, 0x1
    b @L_80071844
@L_800717C4:
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_800717E0
    li r3, 0x2
    b @L_80071844
@L_800717E0:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80071820
    lwz r12, 0x0(r24)
    cmplwi r12, 0x0
    beq @L_8007180C
    mr r3, r30
    lwz r4, 0x0(r25)
    mtctr r12
    bctrl
@L_8007180C:
    lwzx r0, r27, r28
    cmpwi r0, 0x0
    beq @L_800717AC
    li r3, 0x3e8
    b @L_80071844
@L_80071820:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0xa
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80071840
    li r3, 0x3
    b @L_80071844
@L_80071840:
    li r3, 0x0
@L_80071844:
    cmpwi r3, 0x0
    beq @L_80071854
    addi r27, r3, 0xb
    b @L_80071858
@L_80071854:
    li r27, 0x0
@L_80071858:
    cmpwi r27, 0x0
    beq @L_80071864
    b @L_80071984
@L_80071864:
    lwz r0, 0x10(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0x44
    beq @L_8007187C
    li r27, 0xf
    b @L_80071984
@L_8007187C:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r26, r0, 0x7530
    bl OSGetTick
    lis r5, -32709
    lis r4, -32709
    rlwinm r6, r30, 3, 0, 28
    mr r27, r3
    addi r0, r5, 28184
    rlwinm r28, r30, 2, 0, 29
    add r24, r0, r6
    addi r29, r4, 28168
    addi r25, r24, 0x4
@L_800718C4:
    bl OSGetTick
    subf r0, r27, r3
    cmplw r0, r26
    ble @L_800718DC
    li r3, 0x1
    b @L_8007195C
@L_800718DC:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_800718F8
    li r3, 0x2
    b @L_8007195C
@L_800718F8:
    lbz r0, 0x9(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80071938
    lwz r12, 0x0(r24)
    cmplwi r12, 0x0
    beq @L_80071924
    mr r3, r30
    lwz r4, 0x0(r25)
    mtctr r12
    bctrl
@L_80071924:
    lwzx r0, r29, r28
    cmpwi r0, 0x0
    beq @L_800718C4
    li r3, 0x3e8
    b @L_8007195C
@L_80071938:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0xb
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80071958
    li r3, 0x3
    b @L_8007195C
@L_80071958:
    li r3, 0x0
@L_8007195C:
    cmpwi r3, 0x0
    beq @L_8007196C
    addi r27, r3, 0xf
    b @L_80071984
@L_8007196C:
    lwz r0, 0x10(r1)
    cmplwi r0, 0x0
    beq @L_80071980
    li r27, 0x13
    b @L_80071984
@L_80071980:
    li r27, 0x0
@L_80071984:
    mr r3, r31
    li r4, 0x1
    bl fn_8008ABE4
    mr r3, r27
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
