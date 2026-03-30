    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r21, 0x24(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    clrlwi. r0, r0, 31
    beq @L_800D92E4
    lbz r3, 0x60(r3)
    bl fn_800BA6B0
    li r23, 0x0
    li r22, 0x0
    b @L_800D92C0
    @L_800D9270:
    addi r24, r22, 0x61
    mr r3, r23
    add r24, r4, r24
    lbz r4, 0x0(r24)
    lbz r5, 0x1(r24)
    lbz r6, 0x2(r24)
    lbz r7, 0x3(r24)
    lbz r8, 0x4(r24)
    lbz r9, 0x5(r24)
    bl fn_800BA6F4
    lbz r4, 0x6(r24)
    addi r3, r23, 0x2
    lbz r5, 0x7(r24)
    lbz r6, 0x8(r24)
    lbz r7, 0x9(r24)
    lbz r8, 0xa(r24)
    lbz r9, 0xb(r24)
    bl fn_800BA6F4
    addi r22, r22, 0xc
    addi r23, r23, 0x1
    @L_800D92C0:
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x60(r4)
    cmpw r23, r0
    blt @L_800D9270
    lis r3, lbl_804001F0@ha
    addi r4, r3, lbl_804001F0@l
    lwz r3, 0x18(r4)
    addi r0, r3, 0x1
    stw r0, 0x18(r4)
    @L_800D92E4:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800D9310
    lbz r3, 0x79(r3)
    bl fn_800B884C
    lis r3, lbl_804001F0@ha
    addi r4, r3, lbl_804001F0@l
    lwz r3, 0x1c(r4)
    addi r0, r3, 0x1
    stw r0, 0x1c(r4)
    @L_800D9310:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800D961C
    lbz r3, 0x7a(r3)
    bl fn_800BC8C8
    lwz r4, lbl_8047AA80(r13)
    lbz r23, 0x3ac(r4)
    addi r29, r4, 0x7b
    addi r28, r4, 0xab
    addi r27, r4, 0xfb
    mr r3, r23
    addi r26, r4, 0x14b
    addi r25, r4, 0x18b
    addi r24, r4, 0x25c
    addi r22, r4, 0x26c
    bl fn_800BBC0C
    cmplwi r23, 0x0
    beq @L_800D93C4
    lwz r3, lbl_8047AA80(r13)
    li r30, 0x0
    addi r31, r3, 0x3ad
    addi r21, r3, 0x3c0
    b @L_800D9398
    @L_800D9370:
    lbz r4, 0x0(r31)
    mr r3, r30
    lbz r5, 0x1(r31)
    bl fn_800BBAF8
    lbz r4, 0x2(r31)
    mr r3, r30
    lbz r5, 0x3(r31)
    bl fn_800BB97C
    addi r31, r31, 0x4
    addi r30, r30, 0x1
    @L_800D9398:
    cmpw r30, r23
    blt @L_800D9370
    li r30, 0x0
    @L_800D93A4:
    lbz r5, 0x18(r21)
    mr r4, r21
    addi r3, r30, 0x1
    bl fn_800BB81C
    addi r30, r30, 0x1
    addi r21, r21, 0x1c
    cmpwi r30, 0x3
    blt @L_800D93A4
    @L_800D93C4:
    li r30, 0x0
    mr r31, r30
    b @L_800D95AC
    @L_800D93D0:
    lbz r4, 0x0(r29)
    mr r3, r30
    lbz r5, 0x1(r29)
    lbz r6, 0x2(r29)
    bl fn_800BC6F0
    lbz r4, 0x0(r28)
    mr r3, r30
    lbz r5, 0x1(r28)
    lbz r6, 0x2(r28)
    lbz r7, 0x3(r28)
    lbz r8, 0x4(r28)
    bl fn_800BC228
    lbz r4, 0x0(r27)
    mr r3, r30
    lbz r5, 0x1(r27)
    lbz r6, 0x2(r27)
    lbz r7, 0x3(r27)
    lbz r8, 0x4(r27)
    bl fn_800BC290
    lbz r4, 0x0(r26)
    mr r3, r30
    lbz r5, 0x1(r26)
    lbz r6, 0x2(r26)
    lbz r7, 0x3(r26)
    bl fn_800BC1A0
    lbz r4, 0x0(r25)
    mr r3, r30
    lbz r5, 0x1(r25)
    lbz r6, 0x2(r25)
    lbz r7, 0x3(r25)
    bl fn_800BC1E4
    lwz r4, lbl_8047AA80(r13)
    addi r0, r31, 0x1cc
    mr r3, r30
    lwzx r4, r4, r0
    bl fn_800BC454
    lwz r4, lbl_8047AA80(r13)
    addi r0, r31, 0x20c
    mr r3, r30
    lwzx r4, r4, r0
    bl fn_800BC4C0
    lbz r0, 0x0(r24)
    cmplwi r0, 0x0
    beq @L_800D9580
    cmplwi r23, 0x0
    beq @L_800D9580
    lbz r0, 0x0(r22)
    cmpwi r0, 0x3
    beq @L_800D954C
    bge @L_800D94B0
    cmpwi r0, 0x1
    beq @L_800D94F8
    bge @L_800D9514
    cmpwi r0, 0x0
    bge @L_800D94C0
    b @L_800D9588
    @L_800D94B0:
    cmpwi r0, 0x5
    beq @L_800D9574
    bge @L_800D9588
    b @L_800D9560
    @L_800D94C0:
    lbz r0, 0x9(r22)
    mr r3, r30
    stw r0, 0x8(r1)
    lbz r0, 0x5(r22)
    stw r0, 0xc(r1)
    lbz r4, 0x1(r22)
    lbz r5, 0x3(r22)
    lbz r6, 0x4(r22)
    lbz r7, 0x2(r22)
    lbz r8, 0x6(r22)
    lbz r9, 0x7(r22)
    lbz r10, 0x8(r22)
    bl fn_800BB780
    b @L_800D9588
    @L_800D94F8:
    lbz r4, 0x1(r22)
    mr r3, r30
    lbz r5, 0xa(r22)
    lbz r6, 0xb(r22)
    lbz r7, 0x2(r22)
    bl fn_800BBC7C
    b @L_800D9588
    @L_800D9514:
    lbz r0, 0x4(r22)
    mr r3, r30
    stw r0, 0x8(r1)
    lbz r0, 0x5(r22)
    stw r0, 0xc(r1)
    lbz r4, 0x1(r22)
    lhz r5, 0xc(r22)
    lhz r6, 0xe(r22)
    lhz r7, 0x10(r22)
    lhz r8, 0x12(r22)
    lbz r9, 0x3(r22)
    lbz r10, 0x2(r22)
    bl fn_800BBCE0
    b @L_800D9588
    @L_800D954C:
    lbz r4, 0x1(r22)
    mr r3, r30
    lbz r5, 0x2(r22)
    bl fn_800BBE8C
    b @L_800D9588
    @L_800D9560:
    lbz r4, 0x1(r22)
    mr r3, r30
    lbz r5, 0x2(r22)
    bl fn_800BBF98
    b @L_800D9588
    @L_800D9574:
    mr r3, r30
    bl fn_800BBFDC
    b @L_800D9588
    @L_800D9580:
    mr r3, r30
    bl fn_800BBC34
    @L_800D9588:
    addi r29, r29, 0x3
    addi r28, r28, 0x5
    addi r27, r27, 0x5
    addi r26, r26, 0x4
    addi r25, r25, 0x4
    addi r24, r24, 0x1
    addi r22, r22, 0x14
    addi r31, r31, 0x4
    addi r30, r30, 0x1
    @L_800D95AC:
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x7a(r3)
    cmpw r30, r0
    blt @L_800D93D0
    li r23, 0x0
    mr r22, r23
    @L_800D95C4:
    lwz r5, lbl_8047AA80(r13)
    addi r0, r22, 0x24c
    mr r3, r23
    addi r4, r1, 0x10
    lwzx r0, r5, r0
    stw r0, 0x10(r1)
    bl fn_800BC3E0
    addi r23, r23, 0x1
    addi r22, r22, 0x4
    cmpwi r23, 0x4
    blt @L_800D95C4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    li r7, 0x3
    bl fn_800BC580
    lis r3, lbl_804001F0@ha
    addi r4, r3, lbl_804001F0@l
    lwz r3, 0x20(r4)
    addi r0, r3, 0x1
    stw r0, 0x20(r4)
    @L_800D961C:
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x0
    stw r0, 0x414(r3)
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
