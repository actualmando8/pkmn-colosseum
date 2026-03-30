    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r31, r4
    mr r26, r5
    mr r30, r6
    mr r27, r7
    mr r28, r8
    mr r29, r9
    addi r0, r25, 0x2388
    li r4, 0x0
    subf r5, r25, r0
    bl memset
    rlwinm r0, r31, 0, 24, 31
    rlwinm r3, r29, 0, 31, 31
    stb r0, 0x340(r25)
    rlwinm r0, r29, 0, 30, 30
    cmplwi r30, 0x4a
    lbz r5, 0x0(r26)
    lbz r4, 0x1(r26)
    lbz r6, 0x2(r26)
    rlwinm r5, r5, 24, 0, 7
    rlwinm r4, r4, 16, 0, 15
    lbz r7, 0x3(r26)
    rlwinm r6, r6, 8, 0, 23
    or r4, r5, r4
    or r4, r6, r4
    or r4, r7, r4
    stw r4, 0x34c(r25)
    stb r3, 0x344(r25)
    stb r0, 0x343(r25)
    lwz r0, 0x0(r26)
    stw r0, 0x354(r25)
    bne @L_8007B3F0
    lis r3, 0x5053
    addi r0, r3, 0x414a
    stw r0, 0x350(r25)
    b @L_8007B3FC
@L_8007B3F0:
    lis r3, 0x5053
    addi r0, r3, 0x4145
    stw r0, 0x350(r25)
@L_8007B3FC:
    lwz r31, lbl_8047A648(r13)
    li r5, 0x0
    lwz r0, lbl_8047A650(r13)
    add r29, r31, r0
    stw r31, 0x380(r25)
    subf r30, r31, r29
    subi r4, r30, 0x3c
    stw r29, 0x384(r25)
    rlwinm r3, r4, 0, 24, 31
    rlwinm r0, r4, 24, 8, 31
    stb r3, 0x2c(r31)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r4, 16, 16, 31
    stb r3, 0x2d(r31)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r4, 8, 24, 31
    stb r3, 0x2e(r31)
    stb r0, 0x2f(r31)
    li r0, 0x4
    mr r3, r27
    mtctr r0
@L_8007B450:
    lbz r4, 0x0(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x1(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x2(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x3(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x4(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x5(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x6(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    lbz r4, 0x7(r3)
    cmplwi r4, 0x0
    beq @L_8007B518
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    addi r3, r3, 0x8
    bdnz @L_8007B450
@L_8007B518:
    cmplwi r5, 0x20
    subfic r3, r5, 0x20
    li r4, 0x0
    bge @L_8007B59C
    rlwinm r0, r3, 29, 3, 31
    cmplwi r0, 0x0
    mtctr r0
    beq @L_8007B588
@L_8007B538:
    addi r0, r5, 0x4
    stbx r4, r31, r0
    addi r0, r5, 0x5
    stbx r4, r31, r0
    addi r0, r5, 0x6
    stbx r4, r31, r0
    addi r0, r5, 0x7
    stbx r4, r31, r0
    addi r0, r5, 0x8
    stbx r4, r31, r0
    addi r0, r5, 0x9
    stbx r4, r31, r0
    addi r0, r5, 0xa
    stbx r4, r31, r0
    addi r0, r5, 0xb
    addi r5, r5, 0x8
    stbx r4, r31, r0
    bdnz @L_8007B538
    andi. r3, r3, 0x7
    beq @L_8007B59C
@L_8007B588:
    mtctr r3
@L_8007B58C:
    addi r0, r5, 0x4
    addi r5, r5, 0x1
    stbx r4, r31, r0
    bdnz @L_8007B58C
@L_8007B59C:
    li r0, 0x2
    rlwinm r3, r28, 0, 24, 31
    stb r0, 0x28(r31)
    li r6, 0x8
    li r0, 0x0
    mr r4, r26
    stb r3, 0x29(r31)
    subi r3, r29, 0x8
    li r5, 0x4
    stb r6, 0x30(r31)
    stb r0, 0x31(r31)
    stb r0, 0x32(r31)
    stb r0, 0x33(r31)
    bl memcpy
    subi r3, r29, 0x4
    addi r4, r25, 0x354
    li r5, 0x4
    bl memcpy
    lis r3, 0xaa48
    subi r7, r30, 0x4
    subi r6, r3, 0x7bde
    li r5, 0x0
    lis r3, lbl_803FAEF8@ha
    addi r3, r3, lbl_803FAEF8@l
@L_8007B5FC:
    mr r4, r5
    addi r5, r5, 0x1
    addi r0, r4, 0x4
    rlwinm r4, r6, 24, 8, 31
    lbzx r0, r31, r0
    cmplw r5, r7
    xor r0, r6, r0
    rlwinm r0, r0, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    xor r6, r4, r0
    bne @L_8007B5FC
    rlwinm r3, r6, 0, 24, 31
    rlwinm r0, r6, 24, 8, 31
    stb r3, 0x0(r31)
    rlwinm r4, r0, 0, 24, 31
    rlwinm r3, r6, 16, 16, 31
    rlwinm r0, r6, 8, 24, 31
    stb r4, 0x1(r31)
    rlwinm r5, r3, 0, 24, 31
    addi r3, r25, 0x35c
    addi r4, r25, 0x37c
    stb r5, 0x2(r31)
    li r5, 0x1
    stb r0, 0x3(r31)
    bl fn_8009F1D0
    lis r3, -32760
    mr r5, r25
    addi r4, r3, -18780
    addi r6, r25, 0x2388
    addi r3, r25, 0x28
    li r7, 0x2000
    li r8, 0x8
    li r9, 0x0
    bl fn_800A19CC
    addi r3, r25, 0x28
    bl fn_800A1F94
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
