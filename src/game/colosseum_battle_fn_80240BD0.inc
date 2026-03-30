    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    li r6, 0x1
    li r7, 0x1
    stw r0, 0xc4(r1)
    stmw r21, 0x94(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    addi r5, r1, 0x68
    mr r4, r26
    li r29, 0x0
    li r3, 0x0
    bl fn_801F1C18
    mr r30, r3
    mr r4, r26
    addi r5, r1, 0x48
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r31, r3
    addi r24, r1, 0x40
    addi r25, r1, 0x48
    li r22, 0x0
    clrlwi r23, r3, 16
    b @L_80240D08
@L_80240C3C:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x40(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x41(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x42(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x43(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x44(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x45(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x46(r1)
    li r3, 0x0
    b @L_80240CE0
@L_80240CBC:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0x8
    blt @L_80240CDC
    cmplwi r0, 0x9
    bgt @L_80240CDC
    li r0, 0x1
    b @L_80240CF0
@L_80240CDC:
    addi r3, r3, 0x1
@L_80240CE0:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80240CBC
    li r0, 0x0
@L_80240CF0:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240D04
    li r0, 0x1
    b @L_80240D18
@L_80240D04:
    addi r22, r22, 0x1
@L_80240D08:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80240C3C
    li r0, 0x0
@L_80240D18:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240D6C
    mr r4, r26
    li r3, 0x0
    li r5, 0x1b6
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b6
    bl fn_80239EE8
@L_80240D6C:
    addi r24, r1, 0x38
    addi r25, r1, 0x68
    clrlwi r23, r30, 16
    li r22, 0x0
    b @L_80240E4C
@L_80240D80:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x38(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x39(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x3a(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x3b(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x3c(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x3d(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x3e(r1)
    li r3, 0x0
    b @L_80240E24
@L_80240E00:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0x3
    blt @L_80240E20
    cmplwi r0, 0x4
    bgt @L_80240E20
    li r0, 0x1
    b @L_80240E34
@L_80240E20:
    addi r3, r3, 0x1
@L_80240E24:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80240E00
    li r0, 0x0
@L_80240E34:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240E48
    li r0, 0x1
    b @L_80240E5C
@L_80240E48:
    addi r22, r22, 0x1
@L_80240E4C:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80240D80
    li r0, 0x0
@L_80240E5C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240EB0
    mr r3, r29
    mr r4, r26
    li r5, 0x1b7
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b7
    bl fn_80239EE8
@L_80240EB0:
    addi r24, r1, 0x30
    addi r25, r1, 0x48
    clrlwi r23, r31, 16
    li r22, 0x0
    b @L_80240F90
@L_80240EC4:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x30(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x31(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x32(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x33(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x34(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x35(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x36(r1)
    li r3, 0x0
    b @L_80240F68
@L_80240F44:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0xa
    blt @L_80240F64
    cmplwi r0, 0xc
    bgt @L_80240F64
    li r0, 0x1
    b @L_80240F78
@L_80240F64:
    addi r3, r3, 0x1
@L_80240F68:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80240F44
    li r0, 0x0
@L_80240F78:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240F8C
    li r0, 0x1
    b @L_80240FA0
@L_80240F8C:
    addi r22, r22, 0x1
@L_80240F90:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80240EC4
    li r0, 0x0
@L_80240FA0:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80240FF4
    mr r3, r29
    mr r4, r26
    li r5, 0x1b8
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b8
    bl fn_80239EE8
@L_80240FF4:
    addi r24, r1, 0x28
    addi r25, r1, 0x68
    clrlwi r23, r30, 16
    li r22, 0x0
    b @L_802410D4
@L_80241008:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x28(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x29(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x2a(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x2b(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x2c(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x2d(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x2e(r1)
    li r3, 0x0
    b @L_802410AC
@L_80241088:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0x0
    blt @L_802410A8
    cmplwi r0, 0x2
    bgt @L_802410A8
    li r0, 0x1
    b @L_802410BC
@L_802410A8:
    addi r3, r3, 0x1
@L_802410AC:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80241088
    li r0, 0x0
@L_802410BC:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_802410D0
    li r0, 0x1
    b @L_802410E4
@L_802410D0:
    addi r22, r22, 0x1
@L_802410D4:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80241008
    li r0, 0x0
@L_802410E4:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241138
    mr r3, r29
    mr r4, r26
    li r5, 0x1b9
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b9
    bl fn_80239EE8
@L_80241138:
    addi r24, r1, 0x20
    addi r25, r1, 0x68
    clrlwi r23, r30, 16
    li r22, 0x0
    b @L_80241218
@L_8024114C:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x20(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x21(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x22(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x23(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x24(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x25(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x26(r1)
    li r3, 0x0
    b @L_802411F0
@L_802411CC:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0x8
    blt @L_802411EC
    cmplwi r0, 0x9
    bgt @L_802411EC
    li r0, 0x1
    b @L_80241200
@L_802411EC:
    addi r3, r3, 0x1
@L_802411F0:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_802411CC
    li r0, 0x0
@L_80241200:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241214
    li r0, 0x1
    b @L_80241228
@L_80241214:
    addi r22, r22, 0x1
@L_80241218:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_8024114C
    li r0, 0x0
@L_80241228:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_8024127C
    mr r3, r29
    mr r4, r26
    li r5, 0x1ba
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ba
    bl fn_80239EE8
@L_8024127C:
    addi r24, r1, 0x18
    addi r25, r1, 0x48
    clrlwi r23, r31, 16
    li r22, 0x0
    b @L_8024135C
@L_80241290:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r25, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x18(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x19(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x1a(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x1b(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x1c(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x1d(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x1e(r1)
    li r3, 0x0
    b @L_80241334
@L_80241310:
    clrlwi r0, r3, 24
    lbzx r0, r24, r0
    cmplwi r0, 0x3
    blt @L_80241330
    cmplwi r0, 0x4
    bgt @L_80241330
    li r0, 0x1
    b @L_80241344
@L_80241330:
    addi r3, r3, 0x1
@L_80241334:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80241310
    li r0, 0x0
@L_80241344:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241358
    li r0, 0x1
    b @L_8024136C
@L_80241358:
    addi r22, r22, 0x1
@L_8024135C:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80241290
    li r0, 0x0
@L_8024136C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_802413C0
    mr r3, r29
    mr r4, r26
    li r5, 0x1bb
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1bb
    bl fn_80239EE8
@L_802413C0:
    addi r25, r1, 0x10
    addi r23, r1, 0x68
    clrlwi r24, r30, 16
    li r22, 0x0
    b @L_802414A0
@L_802413D4:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r23, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x10(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x11(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0x12(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0x13(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0x14(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0x15(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0x16(r1)
    li r3, 0x0
    b @L_80241478
@L_80241454:
    clrlwi r0, r3, 24
    lbzx r0, r25, r0
    cmplwi r0, 0xa
    blt @L_80241474
    cmplwi r0, 0xc
    bgt @L_80241474
    li r0, 0x1
    b @L_80241488
@L_80241474:
    addi r3, r3, 0x1
@L_80241478:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80241454
    li r0, 0x0
@L_80241488:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_8024149C
    li r0, 0x1
    b @L_802414B0
@L_8024149C:
    addi r22, r22, 0x1
@L_802414A0:
    clrlwi r0, r22, 16
    cmplw r0, r24
    blt @L_802413D4
    li r0, 0x0
@L_802414B0:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241504
    mr r3, r29
    mr r4, r26
    li r5, 0x1bc
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1bc
    bl fn_80239EE8
@L_80241504:
    addi r25, r1, 0x8
    addi r24, r1, 0x48
    clrlwi r30, r31, 16
    li r22, 0x0
    b @L_802415E4
@L_80241518:
    clrlslwi r0, r22, 16, 2
    mr r3, r26
    lwzx r21, r24, r0
    mr r4, r21
    bl fn_80235AA0
    stb r3, 0x8(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235A3C
    stb r3, 0x9(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802359D8
    stb r3, 0xa(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235974
    stb r3, 0xb(r1)
    mr r3, r26
    mr r4, r21
    bl fn_80235910
    stb r3, 0xc(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802358AC
    stb r3, 0xd(r1)
    mr r3, r26
    mr r4, r21
    bl fn_802357CC
    stb r3, 0xe(r1)
    li r3, 0x0
    b @L_802415BC
@L_80241598:
    clrlwi r0, r3, 24
    lbzx r0, r25, r0
    cmplwi r0, 0x0
    blt @L_802415B8
    cmplwi r0, 0x2
    bgt @L_802415B8
    li r0, 0x1
    b @L_802415CC
@L_802415B8:
    addi r3, r3, 0x1
@L_802415BC:
    clrlwi r0, r3, 24
    cmplwi r0, 0x7
    blt @L_80241598
    li r0, 0x0
@L_802415CC:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_802415E0
    li r0, 0x1
    b @L_802415F4
@L_802415E0:
    addi r22, r22, 0x1
@L_802415E4:
    clrlwi r0, r22, 16
    cmplw r0, r30
    blt @L_80241518
    li r0, 0x0
@L_802415F4:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80241648
    mr r3, r29
    mr r4, r26
    li r5, 0x1bd
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1bd
    bl fn_80239EE8
@L_80241648:
    mr r3, r29
    lmw r21, 0x94(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
