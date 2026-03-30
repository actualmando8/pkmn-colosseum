    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    stw r0, 0x124(r1)
    stmw r19, 0xec(r1)
    bl fn_8010CBC0
    mr. r28, r3
    beq @L_8010DDEC
    li r3, 0x1
    bl fn_800DA028
    lis r3, -32704
    addi r3, r3, 19560
    lwz r3, 0x3708(r3)
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800D9ED8
    lis r3, -32704
    lwz r30, 0x0(r28)
    addi r3, r3, 19560
    li r31, 0x0
    lwz r0, 0x3704(r3)
    mulli r0, r0, 0xdc0
    add r3, r3, r0
    addi r29, r3, 0x4
    b @L_8010DDB0
@L_8010D964:
    lhz r0, 0x24(r29)
    rlwinm. r0, r0, 0, 31, 31
    bne @L_8010DDA4
    lhz r0, 0x3c(r30)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_8010DDA4
    mr r4, r31
    addi r3, r1, 0xb8
    bl fn_8010CA30
    mr r4, r31
    addi r3, r1, 0x88
    bl fn_8010C8D0
    lwz r3, 0x24(r30)
    cmplwi r3, 0x0
    beq @L_8010D9AC
    addi r4, r1, 0xb8
    addi r5, r1, 0x88
    bl fn_8010D20C
@L_8010D9AC:
    lwz r19, 0x28(r30)
    cmplwi r19, 0x0
    beq @L_8010DAAC
    lwz r0, -26600(r2)
    addi r20, r1, 0x64
    lwz r27, 0x0(r19)
    li r25, 0x0
    stw r0, 0x18(r1)
    b @L_8010DAA0
@L_8010D9D0:
    mr r24, r20
    mr r23, r27
    li r26, 0x0
@L_8010D9DC:
    mr r4, r23
    mr r5, r24
    addi r3, r1, 0xb8
    bl fn_800A37CC
    addi r26, r26, 0x1
    addi r23, r23, 0xc
    cmpwi r26, 0x3
    addi r24, r24, 0xc
    blt @L_8010D9DC
    li r3, 0x1
    bl fn_800D6A00
    li r21, 0x0
    mr r23, r20
@L_8010DA10:
    addi r26, r21, 0x1
    cmpwi r26, 0x3
    blt @L_8010DA20
    li r26, 0x0
@L_8010DA20:
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, 0x0(r23)
    lfs f2, 0x4(r23)
    lfs f3, 0x8(r23)
    bl fn_800D6680
    lbz r4, 0x18(r1)
    li r3, 0x0
    lbz r5, 0x19(r1)
    lbz r6, 0x1a(r1)
    lbz r7, 0x1b(r1)
    bl fn_800D5CB8
    mulli r0, r26, 0xc
    addi r3, r1, 0x64
    add r3, r3, r0
    lfs f1, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f3, 0x8(r3)
    bl fn_800D6680
    lbz r4, 0x18(r1)
    li r3, 0x0
    lbz r5, 0x19(r1)
    lbz r6, 0x1a(r1)
    lbz r7, 0x1b(r1)
    bl fn_800D5CB8
    bl fn_800D6728
    addi r21, r21, 0x1
    addi r23, r23, 0xc
    cmpwi r21, 0x3
    blt @L_8010DA10
    addi r25, r25, 0x1
    addi r27, r27, 0x34
@L_8010DAA0:
    lwz r0, 0x4(r19)
    cmplw r25, r0
    blt @L_8010D9D0
@L_8010DAAC:
    lwz r27, 0x2c(r30)
    cmplwi r27, 0x0
    beq @L_8010DB54
    lwz r0, -26596(r2)
    li r3, 0x3
    lwz r25, 0x0(r27)
    stw r0, 0x14(r1)
    bl fn_800D6A00
    lbz r22, 0x17(r1)
    li r24, 0x0
    lbz r21, 0x16(r1)
    lbz r20, 0x15(r1)
    lbz r19, 0x14(r1)
    b @L_8010DB48
@L_8010DAE4:
    li r3, 0x3
    bl fn_800D67BC
    li r26, 0x0
    mr r23, r25
@L_8010DAF4:
    mr r4, r23
    addi r3, r1, 0xb8
    addi r5, r1, 0x34
    bl fn_800A37CC
    lfs f1, 0x34(r1)
    lfs f2, 0x38(r1)
    lfs f3, 0x3c(r1)
    bl fn_800D6680
    mr r4, r19
    mr r5, r20
    mr r6, r21
    mr r7, r22
    li r3, 0x0
    bl fn_800D5CB8
    addi r26, r26, 0x1
    addi r23, r23, 0xc
    cmpwi r26, 0x3
    blt @L_8010DAF4
    bl fn_800D6728
    addi r24, r24, 0x1
    addi r25, r25, 0x34
@L_8010DB48:
    lwz r0, 0x4(r27)
    cmplw r24, r0
    blt @L_8010DAE4
@L_8010DB54:
    lwz r27, 0x30(r30)
    cmplwi r27, 0x0
    beq @L_8010DBFC
    lwz r0, -26592(r2)
    li r3, 0x3
    lwz r24, 0x0(r27)
    stw r0, 0x10(r1)
    bl fn_800D6A00
    lbz r20, 0x13(r1)
    li r26, 0x0
    lbz r21, 0x12(r1)
    lbz r22, 0x11(r1)
    lbz r23, 0x10(r1)
    b @L_8010DBF0
@L_8010DB8C:
    li r3, 0x3
    bl fn_800D67BC
    li r25, 0x0
    mr r19, r24
@L_8010DB9C:
    mr r4, r19
    addi r3, r1, 0xb8
    addi r5, r1, 0x28
    bl fn_800A37CC
    lfs f1, 0x28(r1)
    lfs f2, 0x2c(r1)
    lfs f3, 0x30(r1)
    bl fn_800D6680
    mr r4, r23
    mr r5, r22
    mr r6, r21
    mr r7, r20
    li r3, 0x0
    bl fn_800D5CB8
    addi r25, r25, 0x1
    addi r19, r19, 0xc
    cmpwi r25, 0x3
    blt @L_8010DB9C
    bl fn_800D6728
    addi r26, r26, 0x1
    addi r24, r24, 0x34
@L_8010DBF0:
    lwz r0, 0x4(r27)
    cmplw r26, r0
    blt @L_8010DB8C
@L_8010DBFC:
    lwz r21, 0x34(r30)
    cmplwi r21, 0x0
    beq @L_8010DCFC
    lwz r0, -26600(r2)
    addi r22, r1, 0x40
    lwz r24, 0x0(r21)
    li r26, 0x0
    stw r0, 0xc(r1)
    b @L_8010DCF0
@L_8010DC20:
    mr r19, r22
    mr r20, r24
    li r25, 0x0
@L_8010DC2C:
    mr r4, r20
    mr r5, r19
    addi r3, r1, 0xb8
    bl fn_800A37CC
    addi r25, r25, 0x1
    addi r20, r20, 0xc
    cmpwi r25, 0x3
    addi r19, r19, 0xc
    blt @L_8010DC2C
    li r3, 0x1
    bl fn_800D6A00
    li r20, 0x0
    mr r19, r22
@L_8010DC60:
    addi r25, r20, 0x1
    cmpwi r25, 0x3
    blt @L_8010DC70
    li r25, 0x0
@L_8010DC70:
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, 0x0(r19)
    lfs f2, 0x4(r19)
    lfs f3, 0x8(r19)
    bl fn_800D6680
    lbz r4, 0xc(r1)
    li r3, 0x0
    lbz r5, 0xd(r1)
    lbz r6, 0xe(r1)
    lbz r7, 0xf(r1)
    bl fn_800D5CB8
    mulli r0, r25, 0xc
    addi r3, r1, 0x40
    add r3, r3, r0
    lfs f1, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f3, 0x8(r3)
    bl fn_800D6680
    lbz r4, 0xc(r1)
    li r3, 0x0
    lbz r5, 0xd(r1)
    lbz r6, 0xe(r1)
    lbz r7, 0xf(r1)
    bl fn_800D5CB8
    bl fn_800D6728
    addi r20, r20, 0x1
    addi r19, r19, 0xc
    cmpwi r20, 0x3
    blt @L_8010DC60
    addi r26, r26, 0x1
    addi r24, r24, 0x34
@L_8010DCF0:
    lwz r0, 0x4(r21)
    cmplw r26, r0
    blt @L_8010DC20
@L_8010DCFC:
    lwz r27, 0x38(r30)
    cmplwi r27, 0x0
    beq @L_8010DDA4
    lwz r0, -26588(r2)
    li r3, 0x3
    lwz r24, 0x0(r27)
    stw r0, 0x8(r1)
    bl fn_800D6A00
    lbz r20, 0xb(r1)
    li r26, 0x0
    lbz r21, 0xa(r1)
    lbz r22, 0x9(r1)
    lbz r23, 0x8(r1)
    b @L_8010DD98
@L_8010DD34:
    li r3, 0x3
    bl fn_800D67BC
    li r25, 0x0
    mr r19, r24
@L_8010DD44:
    mr r4, r19
    addi r3, r1, 0xb8
    addi r5, r1, 0x1c
    bl fn_800A37CC
    lfs f1, 0x1c(r1)
    lfs f2, 0x20(r1)
    lfs f3, 0x24(r1)
    bl fn_800D6680
    mr r4, r23
    mr r5, r22
    mr r6, r21
    mr r7, r20
    li r3, 0x0
    bl fn_800D5CB8
    addi r25, r25, 0x1
    addi r19, r19, 0xc
    cmpwi r25, 0x3
    blt @L_8010DD44
    bl fn_800D6728
    addi r26, r26, 0x1
    addi r24, r24, 0x30
@L_8010DD98:
    lwz r0, 0x4(r27)
    cmplw r26, r0
    blt @L_8010DD34
@L_8010DDA4:
    addi r31, r31, 0x1
    addi r30, r30, 0x40
    addi r29, r29, 0x28
@L_8010DDB0:
    lwz r0, 0x4(r28)
    cmplw r31, r0
    blt @L_8010D964
    lis r3, -32704
    addi r27, r3, 19560
    lwz r0, 0x370c(r27)
    cmplwi r0, 0x0
    bne @L_8010DDD8
    bl fn_8010D3C8
    stw r3, 0x370c(r27)
@L_8010DDD8:
    lwz r3, 0x370c(r27)
    cmplwi r3, 0x0
    beq @L_8010DDEC
    bl fn_800DAD10
    bl fn_800D30AC
@L_8010DDEC:
    lmw r19, 0xec(r1)
    lwz r0, 0x124(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
