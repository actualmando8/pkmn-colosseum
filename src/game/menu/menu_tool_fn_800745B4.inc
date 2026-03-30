    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r27, r3
    bl fn_8007480C
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_800747F4
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    rlwinm r0, r0, 30, 2, 31
    rlwinm r28, r0, 3, 0, 28
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r27, 3, 0, 28
    mr r21, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r22, r27, 2, 0, 29
    add r30, r0, r6
    addi r23, r4, lbl_803B6E08@l
    addi r31, r30, 0x4
    lis r3, 0x1062
    lis r25, 0x8000
    addi r24, r3, 0x4dd3
@L_8007461C:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r28
    ble @L_80074634
    li r3, 0x16
    b @L_800747F8
@L_80074634:
    lwz r12, 0x0(r30)
    cmplwi r12, 0x0
    beq @L_80074650
    mr r3, r27
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
@L_80074650:
    lwzx r0, r23, r22
    cmpwi r0, 0x0
    beq @L_80074664
    li r3, 0x3e8
    b @L_800747F8
@L_80074664:
    lwz r0, 0xf8(r25)
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r24, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r20, r0, 0x5
    bl OSGetTick
    mr r26, r3
@L_80074680:
    bl OSGetTick
    subf r4, r26, r3
    mr r3, r27
    xor r0, r4, r20
    cntlzw r0, r0
    slw r0, r4, r0
    rlwinm r19, r0, 1, 31, 31
    bl fn_80073C38
    cmpwi r3, 0x1
    bne @L_800746B0
    cmpwi r19, 0x0
    beq @L_80074680
@L_800746B0:
    cmpwi r3, 0x0
    beq @L_800746BC
    b @L_800747E0
@L_800746BC:
    li r0, 0xaa
    mr r3, r27
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_800746E4
    li r3, 0xb
    b @L_800747B8
@L_800746E4:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r19, r0, 0x64
    bl OSGetTick
    mr r20, r3
@L_8007470C:
    bl OSGetTick
    subf r0, r20, r3
    cmplw r0, r19
    ble @L_80074724
    li r3, 0x1
    b @L_800747A4
@L_80074724:
    mr r3, r27
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074740
    li r3, 0x2
    b @L_800747A4
@L_80074740:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80074780
    lwz r12, 0x0(r30)
    cmplwi r12, 0x0
    beq @L_8007476C
    mr r3, r27
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
@L_8007476C:
    lwzx r0, r23, r22
    cmpwi r0, 0x0
    beq @L_8007470C
    li r3, 0x3e8
    b @L_800747A4
@L_80074780:
    mr r3, r27
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_800747A0
    li r3, 0x3
    b @L_800747A4
@L_800747A0:
    li r3, 0x0
@L_800747A4:
    cmpwi r3, 0x0
    beq @L_800747B4
    addi r3, r3, 0xb
    b @L_800747B8
@L_800747B4:
    li r3, 0x0
@L_800747B8:
    cmpwi r3, 0x0
    beq @L_800747C4
    b @L_800747E0
@L_800747C4:
    lwz r0, 0x10(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0xaa
    beq @L_800747DC
    li r3, 0xf
    b @L_800747E0
@L_800747DC:
    li r3, 0x0
@L_800747E0:
    cmpwi r3, 0x0
    bne @L_8007461C
    addi r3, r27, 0x1
    li r4, 0x1
    bl fn_8008ABE4
@L_800747F4:
    mr r3, r29
@L_800747F8:
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
