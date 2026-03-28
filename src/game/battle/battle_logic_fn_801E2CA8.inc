    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    lis r3, -32697
    li r5, 0x3e8
    stw r0, 0x64(r1)
    stmw r17, 0x24(r1)
    addi r30, r3, -23488
    addi r31, r30, 0x0
    addi r7, r30, 0x820
    lwz r0, 0x90(r7)
    lwz r3, 0x38(r31)
    mulli r6, r0, 0x28
    lwz r18, 0x40(r31)
    lwz r4, 0x3c(r31)
    lwz r19, 0x44(r31)
    xor r0, r3, r18
    divwu r29, r6, r5
    xor r5, r4, r19
    or. r0, r5, r0
    bne @L_801E2EBC
    lwz r0, 0x8c(r7)
    li r17, 0x0
    stw r17, 0x44(r31)
    cmplwi r0, 0x2
    stw r17, 0x40(r31)
    bne @L_801E2DE8
    li r20, 0x0
    lwz r18, -21424(r13)
    lwz r17, -21420(r13)
    mr r19, r29
    mr r21, r20
@L_801E2D24:
    mr r3, r18
    mr r4, r17
    mr r5, r19
    addi r6, r1, 0x1c
    bl fn_801E2B74
    lwz r0, 0x1c(r1)
    addc r21, r21, r3
    addze r20, r20
    cmpwi r0, 0x0
    beq @L_801E2DCC
    cmpwi r0, 0x1
    bne @L_801E2DA0
    cmplwi r17, 0x0
    slwi r0, r3, 1
    subf r19, r3, r19
    add r18, r18, r0
    beq @L_801E2D6C
    add r17, r17, r0
@L_801E2D6C:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r31, r0
    stw r21, 0x4(r3)
    stw r20, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E2D24
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E2D24
@L_801E2DA0:
    slwi r19, r19, 1
    mr r3, r18
    mr r5, r19
    li r4, 0x0
    bl memset
    cmplwi r17, 0x0
    beq @L_801E2DCC
    mr r3, r17
    mr r5, r19
    li r4, 0x0
    bl memset
@L_801E2DCC:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r29
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E3468
@L_801E2DE8:
    li r20, 0x0
    lwz r18, -21424(r13)
    mr r19, r29
    mr r21, r20
@L_801E2DF8:
    mr r3, r18
    mr r4, r17
    mr r5, r19
    addi r6, r1, 0x18
    bl fn_801E2B74
    lwz r0, 0x18(r1)
    addc r21, r21, r3
    addze r20, r20
    cmpwi r0, 0x0
    beq @L_801E2EA0
    cmpwi r0, 0x1
    bne @L_801E2E74
    cmplwi r17, 0x0
    slwi r0, r3, 1
    subf r19, r3, r19
    add r18, r18, r0
    beq @L_801E2E40
    add r17, r17, r0
@L_801E2E40:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r31, r0
    stw r21, 0x4(r3)
    stw r20, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E2DF8
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E2DF8
@L_801E2E74:
    slwi r19, r19, 1
    mr r3, r18
    mr r5, r19
    li r4, 0x0
    bl memset
    cmplwi r17, 0x0
    beq @L_801E2EA0
    mr r3, r17
    mr r5, r19
    li r4, 0x0
    bl memset
@L_801E2EA0:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r29
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E3468
@L_801E2EBC:
    mr r24, r29
    mr r6, r29
    li r5, 0x0
    bl __mod2u
    mr r28, r4
    mr r3, r18
    mr r4, r19
    mr r6, r24
    li r5, 0x0
    bl __mod2u
    mr. r26, r4
    bne @L_801E2EF0
    mr r26, r29
@L_801E2EF0:
    cmplw r28, r26
    bge @L_801E3170
    subf r0, r28, r26
    lwz r3, -21424(r13)
    slwi r17, r28, 1
    slwi r19, r0, 1
    add r4, r3, r17
    mr r5, r19
    bl memcpy
    addi r18, r30, 0x820
    lwz r0, 0x8c(r18)
    cmplwi r0, 0x2
    bne @L_801E2F34
    lwz r3, -21420(r13)
    mr r5, r19
    add r4, r3, r17
    bl memcpy
@L_801E2F34:
    addi r17, r30, 0x0
    li r21, 0x0
    lwz r20, 0x28(r17)
    b @L_801E2F84
@L_801E2F44:
    slwi r0, r20, 3
    addi r22, r30, 0x0
    add r22, r22, r0
    mr r6, r24
    lwz r3, 0x0(r22)
    li r5, 0x0
    lwz r4, 0x4(r22)
    bl __mod2u
    subfc r0, r28, r4
    addi r20, r20, 0x1
    stw r0, 0x4(r22)
    subfe r0, r21, r3
    cmpwi r20, 0x5
    stw r0, 0x0(r22)
    blt @L_801E2F84
    li r20, 0x0
@L_801E2F84:
    lwz r0, 0x2c(r17)
    cmpw r20, r0
    bne @L_801E2F44
    srwi r3, r19, 1
    lwz r0, 0x8c(r18)
    subf r20, r3, r29
    li r21, 0x0
    subf r18, r20, r29
    cmplwi r0, 0x2
    lwz r0, -21424(r13)
    clrrwi r3, r19, 1
    stw r18, 0x44(r31)
    add r4, r0, r3
    stw r21, 0x40(r31)
    bne @L_801E309C
    lwz r0, -21420(r13)
    mr r23, r20
    mr r22, r4
    addi r19, r30, 0x0
    add r21, r0, r3
    li r24, 0x0
@L_801E2FD8:
    mr r3, r22
    mr r4, r21
    mr r5, r23
    addi r6, r1, 0x14
    bl fn_801E2B74
    lwz r0, 0x14(r1)
    addc r18, r18, r3
    addze r24, r24
    cmpwi r0, 0x0
    beq @L_801E3080
    cmpwi r0, 0x1
    bne @L_801E3054
    cmplwi r21, 0x0
    slwi r0, r3, 1
    subf r23, r3, r23
    add r22, r22, r0
    beq @L_801E3020
    add r21, r21, r0
@L_801E3020:
    lwz r0, 0x2c(r17)
    slwi r0, r0, 3
    add r3, r19, r0
    stw r18, 0x4(r3)
    stw r24, 0x0(r3)
    lwz r3, 0x2c(r17)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r17)
    blt @L_801E2FD8
    li r0, 0x0
    stw r0, 0x2c(r17)
    b @L_801E2FD8
@L_801E3054:
    slwi r17, r23, 1
    mr r3, r22
    mr r5, r17
    li r4, 0x0
    bl memset
    cmplwi r21, 0x0
    beq @L_801E3080
    mr r3, r21
    mr r5, r17
    li r4, 0x0
    bl memset
@L_801E3080:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r20
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E3468
@L_801E309C:
    mr r22, r20
    mr r19, r4
    addi r24, r30, 0x0
    li r23, 0x0
@L_801E30AC:
    mr r3, r19
    mr r4, r21
    mr r5, r22
    addi r6, r1, 0x10
    bl fn_801E2B74
    lwz r0, 0x10(r1)
    addc r18, r18, r3
    addze r23, r23
    cmpwi r0, 0x0
    beq @L_801E3154
    cmpwi r0, 0x1
    bne @L_801E3128
    cmplwi r21, 0x0
    slwi r0, r3, 1
    subf r22, r3, r22
    add r19, r19, r0
    beq @L_801E30F4
    add r21, r21, r0
@L_801E30F4:
    lwz r0, 0x2c(r17)
    slwi r0, r0, 3
    add r3, r24, r0
    stw r18, 0x4(r3)
    stw r23, 0x0(r3)
    lwz r3, 0x2c(r17)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r17)
    blt @L_801E30AC
    li r0, 0x0
    stw r0, 0x2c(r17)
    b @L_801E30AC
@L_801E3128:
    slwi r17, r22, 1
    mr r3, r19
    mr r5, r17
    li r4, 0x0
    bl memset
    cmplwi r21, 0x0
    beq @L_801E3154
    mr r3, r21
    mr r5, r17
    li r4, 0x0
    bl memset
@L_801E3154:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r20
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E3468
@L_801E3170:
    srwi r18, r29, 2
    lwz r4, -21424(r13)
    mr r5, r18
    addi r3, r30, 0xa0
    bl memcpy
    subf r23, r28, r29
    lwz r3, -21424(r13)
    slwi r25, r28, 1
    slwi r20, r23, 1
    add r4, r3, r25
    mr r5, r20
    bl memcpy
    lwz r0, -21424(r13)
    clrrwi r19, r20, 1
    mr r5, r18
    addi r4, r30, 0xa0
    add r3, r0, r19
    bl memcpy
    addi r22, r30, 0x820
    lwz r0, 0x8c(r22)
    cmplwi r0, 0x2
    bne @L_801E31FC
    lwz r4, -21420(r13)
    mr r5, r18
    addi r3, r30, 0xa0
    bl memcpy
    lwz r3, -21420(r13)
    mr r5, r20
    add r4, r3, r25
    bl memcpy
    lwz r0, -21420(r13)
    mr r5, r18
    addi r4, r30, 0xa0
    add r3, r0, r19
    bl memcpy
@L_801E31FC:
    addi r27, r30, 0x0
    srwi r21, r29, 1
    lwz r18, 0x28(r27)
    li r20, 0x0
    b @L_801E3278
@L_801E3210:
    slwi r0, r18, 3
    addi r19, r30, 0x0
    add r19, r19, r0
    mr r6, r24
    lwz r3, 0x0(r19)
    li r5, 0x0
    lwz r4, 0x4(r19)
    bl __mod2u
    subfc r0, r4, r21
    subfe r0, r3, r20
    subfe r0, r17, r17
    neg. r0, r0
    beq @L_801E3258
    subfc r0, r28, r4
    stw r0, 0x4(r19)
    subfe r0, r20, r3
    stw r0, 0x0(r19)
    b @L_801E3268
@L_801E3258:
    addc r0, r4, r23
    stw r0, 0x4(r19)
    adde r0, r3, r20
    stw r0, 0x0(r19)
@L_801E3268:
    addi r18, r18, 0x1
    cmpwi r18, 0x5
    blt @L_801E3278
    li r18, 0x0
@L_801E3278:
    lwz r0, 0x2c(r27)
    cmpw r18, r0
    bne @L_801E3210
    lwz r0, 0x8c(r22)
    subf r17, r26, r28
    lwz r3, -21424(r13)
    slwi r4, r29, 1
    subf r18, r17, r29
    li r21, 0x0
    add r3, r3, r4
    cmplwi r0, 0x2
    stw r18, 0x44(r31)
    subf r0, r25, r3
    slwi r3, r26, 1
    stw r21, 0x40(r31)
    add r19, r0, r3
    bne @L_801E339C
    lwz r0, -21420(r13)
    mr r22, r17
    addi r20, r30, 0x0
    li r23, 0x0
    add r0, r0, r4
    subf r0, r25, r0
    add r21, r0, r3
@L_801E32D8:
    mr r3, r19
    mr r4, r21
    mr r5, r22
    addi r6, r1, 0xc
    bl fn_801E2B74
    lwz r0, 0xc(r1)
    addc r18, r18, r3
    addze r23, r23
    cmpwi r0, 0x0
    beq @L_801E3380
    cmpwi r0, 0x1
    bne @L_801E3354
    cmplwi r21, 0x0
    slwi r0, r3, 1
    subf r22, r3, r22
    add r19, r19, r0
    beq @L_801E3320
    add r21, r21, r0
@L_801E3320:
    lwz r0, 0x2c(r27)
    slwi r0, r0, 3
    add r3, r20, r0
    stw r18, 0x4(r3)
    stw r23, 0x0(r3)
    lwz r3, 0x2c(r27)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r27)
    blt @L_801E32D8
    li r0, 0x0
    stw r0, 0x2c(r27)
    b @L_801E32D8
@L_801E3354:
    slwi r18, r22, 1
    mr r3, r19
    mr r5, r18
    li r4, 0x0
    bl memset
    cmplwi r21, 0x0
    beq @L_801E3380
    mr r3, r21
    mr r5, r18
    li r4, 0x0
    bl memset
@L_801E3380:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r17
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E3468
@L_801E339C:
    mr r22, r17
    addi r20, r30, 0x0
    li r23, 0x0
@L_801E33A8:
    mr r3, r19
    mr r4, r21
    mr r5, r22
    addi r6, r1, 0x8
    bl fn_801E2B74
    lwz r0, 0x8(r1)
    addc r18, r18, r3
    addze r23, r23
    cmpwi r0, 0x0
    beq @L_801E3450
    cmpwi r0, 0x1
    bne @L_801E3424
    cmplwi r21, 0x0
    slwi r0, r3, 1
    subf r22, r3, r22
    add r19, r19, r0
    beq @L_801E33F0
    add r21, r21, r0
@L_801E33F0:
    lwz r0, 0x2c(r27)
    slwi r0, r0, 3
    add r3, r20, r0
    stw r18, 0x4(r3)
    stw r23, 0x0(r3)
    lwz r3, 0x2c(r27)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r27)
    blt @L_801E33A8
    li r0, 0x0
    stw r0, 0x2c(r27)
    b @L_801E33A8
@L_801E3424:
    slwi r18, r22, 1
    mr r3, r19
    mr r5, r18
    li r4, 0x0
    bl memset
    cmplwi r21, 0x0
    beq @L_801E3450
    mr r3, r21
    mr r5, r18
    li r4, 0x0
    bl memset
@L_801E3450:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r17
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
@L_801E3468:
    li r0, 0x0
    addi r5, r30, 0x0
    slwi r17, r29, 1
    stw r0, 0x34(r5)
    lwz r3, -21424(r13)
    mr r4, r17
    stw r0, 0x30(r5)
    stw r0, 0x3c(r31)
    stw r0, 0x38(r31)
    bl DCFlushRange
    lwz r3, -31520(r13)
    mr r5, r29
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
    addi r3, r30, 0x820
    lwz r0, 0x8c(r3)
    cmplwi r0, 0x2
    bne @L_801E34DC
    lwz r3, -21420(r13)
    mr r4, r17
    bl DCFlushRange
    lwz r3, -31516(r13)
    mr r5, r29
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
@L_801E34DC:
    lmw r17, 0x24(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
