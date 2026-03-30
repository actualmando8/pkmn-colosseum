    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r14, 0x18(r1)
    mr r22, r3
    mr r23, r4
    addi r14, r22, 0x1
    li r4, 0x2
    mr r3, r14
    bl fn_8008ABE4
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r25, r0, 0x64
    bl OSGetTick
    lis r5, -32709
    lis r4, -32709
    rlwinm r6, r22, 3, 0, 28
    mr r28, r3
    addi r0, r5, 28184
    rlwinm r29, r22, 2, 0, 29
    add r26, r0, r6
    addi r30, r4, 28168
    addi r27, r26, 0x4
    lis r3, 0x1062
    lis r15, 0x8000
    addi r31, r3, 0x4dd3
@L_80072724:
    bl OSGetTick
    lwz r0, 0xf8(r15)
    subf r4, r28, r3
    xor r3, r4, r25
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r31, r0
    cntlzw r3, r3
    slw r3, r4, r3
    rlwinm r24, r3, 1, 31, 31
    rlwinm r0, r0, 26, 6, 31
    mulli r17, r0, 0x64
    bl OSGetTick
    mr r16, r3
@L_80072758:
    bl OSGetTick
    subf r0, r16, r3
    cmplw r0, r17
    ble @L_80072770
    li r16, 0x1
    b @L_800729BC
@L_80072770:
    mr r3, r22
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_800727B4
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_800727A0
    mr r3, r22
    lwz r4, 0x0(r27)
    mtctr r12
    bctrl
@L_800727A0:
    lwzx r0, r30, r29
    cmpwi r0, 0x0
    beq @L_80072758
    li r16, 0x3e8
    b @L_800729BC
@L_800727B4:
    mr r3, r22
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_800727CC
    mr r16, r3
    b @L_800729BC
@L_800727CC:
    li r0, 0x55
    mr r3, r22
    stw r0, 0x10(r1)
    addi r4, r1, 0x10
    addi r5, r1, 0xa
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_800727F4
    li r16, 0xb
    b @L_800728C8
@L_800727F4:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r17, r0, 0x64
    bl OSGetTick
    mr r16, r3
@L_8007281C:
    bl OSGetTick
    subf r0, r16, r3
    cmplw r0, r17
    ble @L_80072834
    li r3, 0x1
    b @L_800728B4
@L_80072834:
    mr r3, r22
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80072850
    li r3, 0x2
    b @L_800728B4
@L_80072850:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80072890
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_8007287C
    mr r3, r22
    lwz r4, 0x0(r27)
    mtctr r12
    bctrl
@L_8007287C:
    lwzx r0, r30, r29
    cmpwi r0, 0x0
    beq @L_8007281C
    li r3, 0x3e8
    b @L_800728B4
@L_80072890:
    mr r3, r22
    addi r4, r1, 0x14
    addi r5, r1, 0xa
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_800728B0
    li r3, 0x3
    b @L_800728B4
@L_800728B0:
    li r3, 0x0
@L_800728B4:
    cmpwi r3, 0x0
    beq @L_800728C4
    addi r16, r3, 0xb
    b @L_800728C8
@L_800728C4:
    li r16, 0x0
@L_800728C8:
    cmpwi r16, 0x0
    beq @L_800728D4
    b @L_800729BC
@L_800728D4:
    lwz r0, 0x14(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0x55
    beq @L_800728EC
    li r16, 0xf
    b @L_800729BC
@L_800728EC:
    mr r16, r23
    li r18, 0x0
    lis r3, 0x1062
    lis r20, 0x8000
    addi r19, r3, 0x4dd3
@L_80072900:
    lwz r0, 0x0(r16)
    mr r3, r22
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    bne @L_800729B8
    lwz r0, 0xf8(r20)
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r19, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r17, r0, 0x64
    bl OSGetTick
    mr r21, r3
@L_8007293C:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r17
    bgt @L_800729B8
    mr r3, r22
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_800729B8
    lbz r0, 0x9(r1)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8007299C
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_8007298C
    mr r3, r22
    lwz r4, 0x0(r27)
    mtctr r12
    bctrl
@L_8007298C:
    lwzx r0, r30, r29
    cmpwi r0, 0x0
    beq @L_8007293C
    b @L_800729B8
@L_8007299C:
    lwzx r0, r30, r29
    cmpwi r0, 0x0
    bne @L_800729B8
    addi r16, r16, 0x4
    addi r18, r18, 0x4
    cmpwi r18, 0x78
    blt @L_80072900
@L_800729B8:
    li r16, 0x0
@L_800729BC:
    cmpwi r16, 0x1
    bne @L_800729CC
    cmpwi r24, 0x0
    beq @L_80072724
@L_800729CC:
    cmpwi r16, 0x0
    mr r3, r14
    beq @L_800729E0
    li r4, 0x1
    b @L_800729E4
@L_800729E0:
    li r4, 0x3
@L_800729E4:
    bl fn_8008ABE4
    mr r3, r16
    lmw r14, 0x18(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
