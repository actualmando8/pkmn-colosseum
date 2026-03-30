    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    stw r0, 0x124(r1)
    stmw r20, 0xf0(r1)
    bl fn_8010CBC0
    mr. r29, r3
    bne @L_8010D3EC
    li r3, 0x0
    b @L_8010D8C0
@L_8010D3EC:
    li r3, 0x1
    bl fn_800DA028
    lis r3, -32704
    addi r28, r3, 19560
    lwz r3, 0x3708(r28)
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
    lwz r3, 0x3708(r28)
    lis r4, 0x8
    bl fn_800DAF60
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8010D468
    lis r3, -32729
    addi r3, r3, 8272
    crxor 6, 6, 6
    bl fn_800C8710
    li r3, 0x0
    b @L_8010D8C0
@L_8010D468:
    lwz r30, 0x0(r29)
    li r31, 0x0
    b @L_8010D8B0
@L_8010D474:
    lhz r0, 0x3c(r30)
    rlwinm. r0, r0, 0, 31, 31
    bne @L_8010D8A8
    mr r4, r31
    addi r3, r1, 0xb8
    bl fn_8010CA30
    mr r4, r31
    addi r3, r1, 0x88
    bl fn_8010C8D0
    lwz r3, 0x24(r30)
    cmplwi r3, 0x0
    beq @L_8010D4B0
    addi r4, r1, 0xb8
    addi r5, r1, 0x88
    bl fn_8010D20C
@L_8010D4B0:
    lwz r20, 0x28(r30)
    cmplwi r20, 0x0
    beq @L_8010D5B0
    lwz r0, -26600(r2)
    addi r21, r1, 0x64
    lwz r28, 0x0(r20)
    li r26, 0x0
    stw r0, 0x18(r1)
    b @L_8010D5A4
@L_8010D4D4:
    mr r25, r21
    mr r24, r28
    li r27, 0x0
@L_8010D4E0:
    mr r4, r24
    mr r5, r25
    addi r3, r1, 0xb8
    bl fn_800A37CC
    addi r27, r27, 0x1
    addi r24, r24, 0xc
    cmpwi r27, 0x3
    addi r25, r25, 0xc
    blt @L_8010D4E0
    li r3, 0x1
    bl fn_800D6A00
    li r22, 0x0
    mr r24, r21
@L_8010D514:
    addi r27, r22, 0x1
    cmpwi r27, 0x3
    blt @L_8010D524
    li r27, 0x0
@L_8010D524:
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, 0x0(r24)
    lfs f2, 0x4(r24)
    lfs f3, 0x8(r24)
    bl fn_800D6680
    lbz r4, 0x18(r1)
    li r3, 0x0
    lbz r5, 0x19(r1)
    lbz r6, 0x1a(r1)
    lbz r7, 0x1b(r1)
    bl fn_800D5CB8
    mulli r0, r27, 0xc
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
    addi r22, r22, 0x1
    addi r24, r24, 0xc
    cmpwi r22, 0x3
    blt @L_8010D514
    addi r26, r26, 0x1
    addi r28, r28, 0x34
@L_8010D5A4:
    lwz r0, 0x4(r20)
    cmplw r26, r0
    blt @L_8010D4D4
@L_8010D5B0:
    lwz r28, 0x2c(r30)
    cmplwi r28, 0x0
    beq @L_8010D658
    lwz r0, -26596(r2)
    li r3, 0x3
    lwz r26, 0x0(r28)
    stw r0, 0x14(r1)
    bl fn_800D6A00
    lbz r23, 0x17(r1)
    li r25, 0x0
    lbz r22, 0x16(r1)
    lbz r21, 0x15(r1)
    lbz r20, 0x14(r1)
    b @L_8010D64C
@L_8010D5E8:
    li r3, 0x3
    bl fn_800D67BC
    li r27, 0x0
    mr r24, r26
@L_8010D5F8:
    mr r4, r24
    addi r3, r1, 0xb8
    addi r5, r1, 0x34
    bl fn_800A37CC
    lfs f1, 0x34(r1)
    lfs f2, 0x38(r1)
    lfs f3, 0x3c(r1)
    bl fn_800D6680
    mr r4, r20
    mr r5, r21
    mr r6, r22
    mr r7, r23
    li r3, 0x0
    bl fn_800D5CB8
    addi r27, r27, 0x1
    addi r24, r24, 0xc
    cmpwi r27, 0x3
    blt @L_8010D5F8
    bl fn_800D6728
    addi r25, r25, 0x1
    addi r26, r26, 0x34
@L_8010D64C:
    lwz r0, 0x4(r28)
    cmplw r25, r0
    blt @L_8010D5E8
@L_8010D658:
    lwz r28, 0x30(r30)
    cmplwi r28, 0x0
    beq @L_8010D700
    lwz r0, -26592(r2)
    li r3, 0x3
    lwz r25, 0x0(r28)
    stw r0, 0x10(r1)
    bl fn_800D6A00
    lbz r21, 0x13(r1)
    li r27, 0x0
    lbz r22, 0x12(r1)
    lbz r23, 0x11(r1)
    lbz r24, 0x10(r1)
    b @L_8010D6F4
@L_8010D690:
    li r3, 0x3
    bl fn_800D67BC
    li r26, 0x0
    mr r20, r25
@L_8010D6A0:
    mr r4, r20
    addi r3, r1, 0xb8
    addi r5, r1, 0x28
    bl fn_800A37CC
    lfs f1, 0x28(r1)
    lfs f2, 0x2c(r1)
    lfs f3, 0x30(r1)
    bl fn_800D6680
    mr r4, r24
    mr r5, r23
    mr r6, r22
    mr r7, r21
    li r3, 0x0
    bl fn_800D5CB8
    addi r26, r26, 0x1
    addi r20, r20, 0xc
    cmpwi r26, 0x3
    blt @L_8010D6A0
    bl fn_800D6728
    addi r27, r27, 0x1
    addi r25, r25, 0x34
@L_8010D6F4:
    lwz r0, 0x4(r28)
    cmplw r27, r0
    blt @L_8010D690
@L_8010D700:
    lwz r22, 0x34(r30)
    cmplwi r22, 0x0
    beq @L_8010D800
    lwz r0, -26600(r2)
    addi r23, r1, 0x40
    lwz r25, 0x0(r22)
    li r27, 0x0
    stw r0, 0xc(r1)
    b @L_8010D7F4
@L_8010D724:
    mr r20, r23
    mr r21, r25
    li r26, 0x0
@L_8010D730:
    mr r4, r21
    mr r5, r20
    addi r3, r1, 0xb8
    bl fn_800A37CC
    addi r26, r26, 0x1
    addi r21, r21, 0xc
    cmpwi r26, 0x3
    addi r20, r20, 0xc
    blt @L_8010D730
    li r3, 0x1
    bl fn_800D6A00
    li r21, 0x0
    mr r20, r23
@L_8010D764:
    addi r26, r21, 0x1
    cmpwi r26, 0x3
    blt @L_8010D774
    li r26, 0x0
@L_8010D774:
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, 0x0(r20)
    lfs f2, 0x4(r20)
    lfs f3, 0x8(r20)
    bl fn_800D6680
    lbz r4, 0xc(r1)
    li r3, 0x0
    lbz r5, 0xd(r1)
    lbz r6, 0xe(r1)
    lbz r7, 0xf(r1)
    bl fn_800D5CB8
    mulli r0, r26, 0xc
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
    addi r21, r21, 0x1
    addi r20, r20, 0xc
    cmpwi r21, 0x3
    blt @L_8010D764
    addi r27, r27, 0x1
    addi r25, r25, 0x34
@L_8010D7F4:
    lwz r0, 0x4(r22)
    cmplw r27, r0
    blt @L_8010D724
@L_8010D800:
    lwz r28, 0x38(r30)
    cmplwi r28, 0x0
    beq @L_8010D8A8
    lwz r0, -26588(r2)
    li r3, 0x3
    lwz r25, 0x0(r28)
    stw r0, 0x8(r1)
    bl fn_800D6A00
    lbz r21, 0xb(r1)
    li r27, 0x0
    lbz r22, 0xa(r1)
    lbz r23, 0x9(r1)
    lbz r24, 0x8(r1)
    b @L_8010D89C
@L_8010D838:
    li r3, 0x3
    bl fn_800D67BC
    li r26, 0x0
    mr r20, r25
@L_8010D848:
    mr r4, r20
    addi r3, r1, 0xb8
    addi r5, r1, 0x1c
    bl fn_800A37CC
    lfs f1, 0x1c(r1)
    lfs f2, 0x20(r1)
    lfs f3, 0x24(r1)
    bl fn_800D6680
    mr r4, r24
    mr r5, r23
    mr r6, r22
    mr r7, r21
    li r3, 0x0
    bl fn_800D5CB8
    addi r26, r26, 0x1
    addi r20, r20, 0xc
    cmpwi r26, 0x3
    blt @L_8010D848
    bl fn_800D6728
    addi r27, r27, 0x1
    addi r25, r25, 0x30
@L_8010D89C:
    lwz r0, 0x4(r28)
    cmplw r27, r0
    blt @L_8010D838
@L_8010D8A8:
    addi r31, r31, 0x1
    addi r30, r30, 0x40
@L_8010D8B0:
    lwz r0, 0x4(r29)
    cmplw r31, r0
    blt @L_8010D474
    bl fn_800DADB4
@L_8010D8C0:
    lmw r20, 0xf0(r1)
    lwz r0, 0x124(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
