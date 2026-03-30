    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stmw r21, 0x84(r1)
    mr r30, r3
    lbz r0, 0x2(r30)
    lis r4, lbl_80267EA8@ha
    lwz r3, lbl_8047E708(r2)
    addi r28, r4, lbl_80267EA8@l
    extsb r0, r0
    li r31, 0x1
    cmpwi r0, 0x0
    stw r3, 0x8(r1)
    bne @L_8006E450
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_8006E3F0
    bge @L_8006E450
    cmpwi r0, 0x0
    beq @L_8006E390
    b @L_8006E450
    @L_8006E390:
    addi r26, r28, 0x698
    addi r27, r28, 0x628
    li r21, 0x0
    addi r25, r28, 0x0
    @L_8006E3A0:
    lwz r0, 0x0(r26)
    mr r24, r27
    li r22, 0x0
    slwi r23, r0, 2
    @L_8006E3B0:
    lhz r4, 0x0(r24)
    mr r3, r30
    bl fn_801046C8
    lhzx r4, r25, r23
    addi r3, r3, 0xc
    bl fn_80108518
    addi r24, r24, 0x2
    addi r22, r22, 0x1
    cmplwi r22, 0xe
    blt @L_8006E3B0
    addi r26, r26, 0x4
    addi r27, r27, 0x1c
    addi r21, r21, 0x1
    cmplwi r21, 0x4
    blt @L_8006E3A0
    b @L_8006E450
    @L_8006E3F0:
    addi r26, r28, 0x698
    addi r27, r28, 0x628
    li r22, 0x0
    addi r23, r28, 0x0
    @L_8006E400:
    lwz r0, 0x0(r26)
    mr r24, r27
    li r21, 0x0
    slwi r3, r0, 2
    addi r25, r3, 0x2
    @L_8006E414:
    lhz r4, 0x0(r24)
    mr r3, r30
    bl fn_801046C8
    lhzx r4, r23, r25
    addi r3, r3, 0xc
    bl fn_80108518
    addi r24, r24, 0x2
    addi r21, r21, 0x1
    cmplwi r21, 0xe
    blt @L_8006E414
    addi r26, r26, 0x4
    addi r27, r27, 0x1c
    addi r22, r22, 0x1
    cmplwi r22, 0x4
    blt @L_8006E400
    @L_8006E450:
    li r24, 0x0
    addi r26, r28, 0x6a8
    mr r29, r24
    la r27, lbl_8047C058(r2)
    @L_8006E460:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r29, 0x59cc
    lwzx r3, r3, r0
    bl fn_8006B154
    cmpwi r3, 0x0
    blt @L_8006E570
    mulli r0, r3, 0x1c
    addi r23, r28, 0x628
    addi r4, r1, 0x8
    li r5, 0x1
    stbx r5, r4, r3
    add r23, r23, r0
    addi r25, r1, 0x44
    li r21, 0x0
    @L_8006E4A0:
    lhz r4, 0x0(r23)
    mr r3, r30
    bl fn_801046C8
    stw r3, 0x0(r25)
    addi r23, r23, 0x2
    addi r25, r25, 0x4
    addi r21, r21, 0x1
    cmplwi r21, 0xe
    blt @L_8006E4A0
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r29, 0x59a8
    add r3, r3, r0
    bl fn_8006A7E8
    cmpwi r3, 0x0
    beq @L_8006E4EC
    li r3, 0x2b2
    b @L_8006E4F0
    @L_8006E4EC:
    li r3, 0x2ae
    @L_8006E4F0:
    bl fn_8005D858
    mr r0, r3
    lwz r3, 0x54(r1)
    mr r4, r0
    bl fn_80071318
    lhz r3, 0x0(r27)
    bl fn_8005D858
    mr r4, r3
    lwz r3, 0x58(r1)
    bl fn_80071318
    lwz r6, 0x44(r1)
    li r3, 0x0
    lwz r5, 0x0(r26)
    li r4, 0xe
    lwz r0, 0x64(r6)
    lwz r8, 0x48(r1)
    clrlwi r0, r0, 24
    lwz r7, 0x4c(r1)
    or r0, r0, r5
    stw r0, 0x64(r6)
    lwz r0, 0x64(r8)
    clrlwi r0, r0, 24
    or r0, r0, r5
    stw r0, 0x64(r8)
    lwz r0, 0x64(r7)
    clrlwi r0, r0, 24
    or r0, r0, r5
    stw r0, 0x64(r7)
    bl fn_80129280
    addi r0, r29, 0x7005
    lbzx r0, r3, r0
    and r31, r31, r0
    @L_8006E570:
    addi r29, r29, 0x1660
    addi r27, r27, 0x2
    addi r26, r26, 0x4
    addi r24, r24, 0x1
    cmpwi r24, 0x4
    blt @L_8006E460
    clrlwi r0, r31, 24
    addi r26, r28, 0x628
    cntlzw r0, r0
    addi r27, r1, 0x8
    srwi r29, r0, 5
    li r24, 0x0
    addi r25, r1, 0xc
    @L_8006E5A4:
    mr r23, r26
    mr r22, r25
    li r21, 0x0
    @L_8006E5B0:
    lhz r4, 0x0(r23)
    mr r3, r30
    bl fn_801046C8
    stw r3, 0x0(r22)
    addi r23, r23, 0x2
    addi r22, r22, 0x4
    addi r21, r21, 0x1
    cmplwi r21, 0xe
    blt @L_8006E5B0
    lbz r0, 0x0(r27)
    cmplwi r0, 0x0
    beq @L_8006E6D4
    li r23, 0x0
    mr r22, r23
    @L_8006E5E8:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r22, 0x59cc
    lwzx r3, r3, r0
    bl fn_8006B154
    cmpw r24, r3
    beq @L_8006E618
    addi r22, r22, 0x1660
    addi r23, r23, 0x1
    cmpwi r23, 0x4
    blt @L_8006E5E8
    @L_8006E618:
    cmplwi r23, 0x4
    blt @L_8006E630
    addi r3, r28, 0x7d8
    addi r5, r28, 0x83c
    li r4, 0x8a1
    bl fn_80196E10
    @L_8006E630:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r23, 0x1660
    addi r0, r4, 0x7005
    lbzx r21, r3, r0
    cmplwi r21, 0x0
    beq @L_8006E658
    li r0, 0x0
    b @L_8006E65C
    @L_8006E658:
    li r0, 0x3f3f
    @L_8006E65C:
    lwz r3, 0x34(r1)
    cmplwi r21, 0x0
    stw r0, 0x4c(r3)
    beq @L_8006E674
    li r0, 0x0
    b @L_8006E678
    @L_8006E674:
    li r0, 0x3f40
    @L_8006E678:
    lwz r5, 0x30(r1)
    clrlwi r4, r29, 24
    lwz r3, 0x14(r1)
    stw r0, 0x4c(r5)
    bl fn_80109220
    cntlzw r0, r21
    lwz r3, 0x2c(r1)
    srwi r23, r0, 5
    clrlwi r4, r23, 24
    bl fn_80109220
    lwz r3, 0x28(r1)
    clrlwi r4, r23, 24
    bl fn_80109220
    cmplwi r21, 0x0
    lwz r3, 0x40(r1)
    li r0, 0x0
    beq @L_8006E6C8
    cmpwi r29, 0x0
    beq @L_8006E6C8
    li r0, 0x1
    @L_8006E6C8:
    clrlwi r4, r0, 24
    bl fn_80109220
    b @L_8006E6F8
    @L_8006E6D4:
    mr r22, r25
    li r23, 0x0
    @L_8006E6DC:
    lwz r3, 0x0(r22)
    li r4, 0x0
    bl fn_80109220
    addi r22, r22, 0x4
    addi r23, r23, 0x1
    cmplwi r23, 0xe
    blt @L_8006E6DC
    @L_8006E6F8:
    addi r26, r26, 0x1c
    addi r27, r27, 0x1
    addi r24, r24, 0x1
    cmplwi r24, 0x4
    blt @L_8006E5A4
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x59cc(r3)
    cmpwi r0, 0x1
    beq @L_8006E770
    li r3, 0x2ae
    bl fn_8005D858
    addi r4, r28, 0x628
    mr r21, r3
    lhz r4, 0x8(r4)
    mr r3, r30
    bl fn_801046C8
    mr r4, r21
    mr r21, r3
    bl fn_80071318
    mr r3, r21
    li r4, 0x1
    bl fn_80109220
    addi r4, r28, 0x628
    mr r3, r30
    lhz r4, 0xc(r4)
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    @L_8006E770:
    mr r3, r30
    li r4, 0x0
    li r5, 0x0
    bl fn_80070D84
    stb r31, 0x98(r30)
    lmw r21, 0x84(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
