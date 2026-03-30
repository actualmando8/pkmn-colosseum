    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0xb4(r1)
    li r0, 0x0
    stmw r14, 0x68(r1)
    mr r15, r4
    mr r16, r7
    li r22, 0x0
    stw r3, 0x5c(r1)
    li r3, 0x0
    li r21, 0x0
    li r20, 0x0
    stw r5, 0x10(r1)
    li r19, 0x0
    li r18, 0x0
    li r17, 0x0
    stw r6, 0x14(r1)
    li r5, 0x0
    stw r3, 0x58(r1)
    lwz r31, 0xa8(r4)
    addi r30, r31, 0x8
    b @L_801BD004
@L_801BCF8C:
    lbzx r3, r30, r5
    subi r4, r3, 0x80
    cmplwi r4, 0x8
    bgt @L_801BD000
    lis r3, jumptable_8036D6F0@ha
    rlwinm r4, r4, 2, 0, 29
    addi r3, r3, jumptable_8036D6F0@l
    lwzx r3, r3, r4
    mtctr r3
    bctr
    li r0, 0x1
    b @L_801BD000
    li r22, 0x1
    b @L_801BD000
    li r21, 0x1
    b @L_801BD000
    li r20, 0x1
    b @L_801BD000
    li r19, 0x1
    b @L_801BD000
    li r3, 0x1
    stw r3, 0x5c(r1)
    b @L_801BD000
    li r18, 0x1
    b @L_801BD000
    li r3, 0x1
    stw r3, 0x58(r1)
    b @L_801BD000
    li r17, 0x1
@L_801BD000:
    addi r5, r5, 0x1
@L_801BD004:
    cmpwi r5, 0x4
    blt @L_801BCF8C
    addi r30, r31, 0xc
    li r5, 0x0
    b @L_801BD080
@L_801BD018:
    lbzx r3, r30, r5
    cmpwi r3, 0x43
    beq @L_801BD068
    bge @L_801BD040
    cmpwi r3, 0x41
    beq @L_801BD058
    bge @L_801BD060
    cmpwi r3, 0x40
    bge @L_801BD050
    b @L_801BD07C
@L_801BD040:
    cmpwi r3, 0x45
    beq @L_801BD078
    bge @L_801BD07C
    b @L_801BD070
@L_801BD050:
    li r22, 0x1
    b @L_801BD07C
@L_801BD058:
    li r21, 0x1
    b @L_801BD07C
@L_801BD060:
    li r20, 0x1
    b @L_801BD07C
@L_801BD068:
    li r19, 0x1
    b @L_801BD07C
@L_801BD070:
    li r18, 0x1
    b @L_801BD07C
@L_801BD078:
    li r17, 0x1
@L_801BD07C:
    addi r5, r5, 0x1
@L_801BD080:
    cmpwi r5, 0x4
    blt @L_801BD018
    cmpwi r0, 0x0
    beq @L_801BD0AC
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x10
    bl fn_801B6F5C
    stw r3, 0x60(r1)
@L_801BD0AC:
    cmpwi r22, 0x0
    beq @L_801BD0D0
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x10
    bl fn_801B6F5C
    mr r29, r3
@L_801BD0D0:
    cmpwi r21, 0x0
    beq @L_801BD0F4
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x11
    bl fn_801B6F5C
    mr r28, r3
@L_801BD0F4:
    cmpwi r20, 0x0
    beq @L_801BD118
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x12
    bl fn_801B6F5C
    mr r27, r3
@L_801BD118:
    cmpwi r19, 0x0
    beq @L_801BD13C
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x13
    bl fn_801B6F5C
    mr r26, r3
@L_801BD13C:
    lwz r0, 0x5c(r1)
    cmpwi r0, 0x0
    beq @L_801BD164
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x14
    bl fn_801B6F5C
    mr r14, r3
@L_801BD164:
    cmpwi r18, 0x0
    beq @L_801BD188
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x17
    bl fn_801B6F5C
    mr r25, r3
@L_801BD188:
    lwz r0, 0x58(r1)
    cmpwi r0, 0x0
    beq @L_801BD1B0
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x18
    bl fn_801B6F5C
    mr r24, r3
@L_801BD1B0:
    cmpwi r17, 0x0
    beq @L_801BD1D4
    lwz r3, 0xa8(r15)
    mr r6, r16
    li r4, 0x6
    li r5, 0x0
    addi r3, r3, 0x1b
    bl fn_801B6F5C
    mr r23, r3
@L_801BD1D4:
    mr r3, r16
    bl fn_801B707C
    mr r4, r15
    mr r15, r3
    li r5, 0xff
    bl fn_801B5E40
    lwz r0, 0x1c(r31)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_801BD62C
    addi r30, r31, 0x8
    li r18, 0x0
    b @L_801BD5D8
@L_801BD204:
    lbzx r0, r30, r18
    cmpwi r0, 0x80
    beq @L_801BD340
    bge @L_801BD250
    cmpwi r0, 0xc
    beq @L_801BD2B0
    bge @L_801BD238
    cmpwi r0, 0x9
    beq @L_801BD31C
    bge @L_801BD5C4
    cmpwi r0, 0x8
    bge @L_801BD2F8
    b @L_801BD5C4
@L_801BD238:
    cmpwi r0, 0xf
    beq @L_801BD28C
    bge @L_801BD5C4
    cmpwi r0, 0xe
    bge @L_801BD5C4
    b @L_801BD2D4
@L_801BD250:
    cmpwi r0, 0x85
    beq @L_801BD3E4
    bge @L_801BD274
    cmpwi r0, 0x83
    beq @L_801BD3A4
    bge @L_801BD3C4
    cmpwi r0, 0x82
    bge @L_801BD384
    b @L_801BD364
@L_801BD274:
    cmpwi r0, 0x88
    beq @L_801BD54C
    bge @L_801BD5C4
    cmpwi r0, 0x87
    bge @L_801BD4D4
    b @L_801BD45C
@L_801BD28C:
    rlwinm r6, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r7, 0x7
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    li r4, 0x0
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD5D4
@L_801BD2B0:
    rlwinm r6, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r7, 0x8
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    li r4, 0x0
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD5D4
@L_801BD2D4:
    rlwinm r6, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r7, 0xc
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    li r4, 0x0
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD5D4
@L_801BD2F8:
    rlwinm r6, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r7, 0x1
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    li r4, -0x1
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD5D4
@L_801BD31C:
    rlwinm r6, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r7, 0x5
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    li r4, -0x1
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD5D4
@L_801BD340:
    rlwinm r0, r18, 2, 0, 29
    addi r5, r1, 0x48
    li r6, 0x1
    rlwinm r4, r18, 2, 0, 29
    stwx r6, r5, r0
    addi r3, r1, 0x38
    lwz r0, 0x60(r1)
    stwx r0, r3, r4
    b @L_801BD5D4
@L_801BD364:
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x6
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r29, r3, r0
    b @L_801BD5D4
@L_801BD384:
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x6
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r28, r3, r0
    b @L_801BD5D4
@L_801BD3A4:
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x6
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r27, r3, r0
    b @L_801BD5D4
@L_801BD3C4:
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x6
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r26, r3, r0
    b @L_801BD5D4
@L_801BD3E4:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r14, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x1
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD5D4
@L_801BD45C:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r25, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B64EC
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x1
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD5D4
@L_801BD4D4:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r24, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x1
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD5D4
@L_801BD54C:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r23, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B64EC
    rlwinm r5, r18, 2, 0, 29
    addi r4, r1, 0x48
    li r6, 0x1
    rlwinm r0, r18, 2, 0, 29
    addi r3, r1, 0x38
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD5D4
@L_801BD5C4:
    addi r3, r2, -22512
    li r4, 0x52f
    addi r5, r2, -22480
    bl fn_80196E10
@L_801BD5D4:
    addi r18, r18, 0x1
@L_801BD5D8:
    cmpwi r18, 0x4
    blt @L_801BD204
    lbz r4, 0x0(r31)
    mr r3, r15
    lbz r5, 0x2(r31)
    lbz r6, 0x4(r31)
    lbz r7, 0x6(r31)
    bl fn_801B6E74
    lwz r0, 0x44(r1)
    mr r3, r15
    lwz r4, 0x48(r1)
    stw r0, 0x8(r1)
    lwz r5, 0x38(r1)
    lwz r6, 0x4c(r1)
    lwz r7, 0x3c(r1)
    lwz r8, 0x50(r1)
    lwz r9, 0x40(r1)
    lwz r10, 0x54(r1)
    bl fn_801B64EC
    lwz r3, 0x10(r1)
    stw r15, 0x0(r3)
@L_801BD62C:
    lwz r0, 0x1c(r31)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801BD8BC
    addi r30, r31, 0xc
    li r14, 0x0
    b @L_801BD868
@L_801BD644:
    lbzx r0, r30, r14
    cmpwi r0, 0x41
    beq @L_801BD704
    bge @L_801BD678
    cmpwi r0, 0x7
    beq @L_801BD69C
    bge @L_801BD66C
    cmpwi r0, 0x4
    beq @L_801BD6C0
    b @L_801BD854
@L_801BD66C:
    cmpwi r0, 0x40
    bge @L_801BD6E4
    b @L_801BD854
@L_801BD678:
    cmpwi r0, 0x44
    beq @L_801BD764
    bge @L_801BD690
    cmpwi r0, 0x43
    bge @L_801BD744
    b @L_801BD724
@L_801BD690:
    cmpwi r0, 0x46
    bge @L_801BD854
    b @L_801BD7DC
@L_801BD69C:
    rlwinm r6, r14, 2, 0, 29
    addi r5, r1, 0x28
    li r7, 0x7
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    li r4, 0x0
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD864
@L_801BD6C0:
    rlwinm r6, r14, 2, 0, 29
    addi r5, r1, 0x28
    li r7, 0x5
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    li r4, -0x1
    stwx r7, r5, r6
    stwx r4, r3, r0
    b @L_801BD864
@L_801BD6E4:
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x6
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r29, r3, r0
    b @L_801BD864
@L_801BD704:
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x6
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r28, r3, r0
    b @L_801BD864
@L_801BD724:
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x6
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r27, r3, r0
    b @L_801BD864
@L_801BD744:
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x6
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r26, r3, r0
    b @L_801BD864
@L_801BD764:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    stw r25, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B5F08
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x5
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD864
@L_801BD7DC:
    mr r3, r16
    bl fn_801B707C
    li r4, 0x0
    mr r17, r3
    li r5, 0xff
    bl fn_801B5E40
    mr r3, r17
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    stw r23, 0x8(r1)
    mr r3, r17
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B5F08
    rlwinm r5, r14, 2, 0, 29
    addi r4, r1, 0x28
    li r6, 0x5
    rlwinm r0, r14, 2, 0, 29
    addi r3, r1, 0x18
    stwx r6, r4, r5
    stwx r17, r3, r0
    b @L_801BD864
@L_801BD854:
    addi r3, r2, -22512
    li r4, 0x578
    addi r5, r2, -22480
    bl fn_80196E10
@L_801BD864:
    addi r14, r14, 0x1
@L_801BD868:
    cmpwi r14, 0x4
    blt @L_801BD644
    lbz r4, 0x1(r31)
    mr r3, r15
    lbz r5, 0x3(r31)
    lbz r6, 0x5(r31)
    lbz r7, 0x7(r31)
    bl fn_801B6CD8
    lwz r0, 0x24(r1)
    mr r3, r15
    lwz r4, 0x28(r1)
    stw r0, 0x8(r1)
    lwz r5, 0x18(r1)
    lwz r6, 0x2c(r1)
    lwz r7, 0x1c(r1)
    lwz r8, 0x30(r1)
    lwz r9, 0x20(r1)
    lwz r10, 0x34(r1)
    bl fn_801B5F08
    lwz r3, 0x14(r1)
    stw r15, 0x0(r3)
@L_801BD8BC:
    lmw r14, 0x68(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr

