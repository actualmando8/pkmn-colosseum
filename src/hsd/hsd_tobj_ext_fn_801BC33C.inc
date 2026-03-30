    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr. r25, r3
    li r30, 0x0
    li r29, 0x0
    li r28, 0x8
    li r27, 0x0
    li r26, 0x0
    bne @L_801BC370
    li r3, 0x0
    b @L_801BC8A8
@L_801BC370:
    mr r31, r25
    b @L_801BC3A4
@L_801BC378:
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmplwi r0, 0x4
    bne @L_801BC390
    mr r26, r31
    b @L_801BC3A0
@L_801BC390:
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BC3A0
    mr r27, r31
@L_801BC3A0:
    lwz r31, 0x8(r31)
@L_801BC3A4:
    cmplwi r31, 0x0
    bne @L_801BC378
    cmplwi r26, 0x0
    beq @L_801BC3B8
    subi r28, r28, 0x1
@L_801BC3B8:
    cmplwi r27, 0x0
    beq @L_801BC3C4
    subi r28, r28, 0x2
@L_801BC3C4:
    mr r31, r25
    b @L_801BC5D0
@L_801BC3CC:
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmplwi r0, 0x4
    bne @L_801BC3F0
    cmplw r31, r26
    beq @L_801BC5CC
    li r0, 0xff
    stw r0, 0xc(r31)
    b @L_801BC5CC
@L_801BC3F0:
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BC410
    cmplw r31, r27
    beq @L_801BC5CC
    li r0, 0xff
    stw r0, 0xc(r31)
    b @L_801BC5CC
@L_801BC410:
    cmplw r30, r28
    bge @L_801BC5C4
    mr r0, r30
    addi r30, r30, 0x1
    cmplwi r0, 0x7
    bgt @L_801BC480
    lis r3, jumptable_8036D6D0@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D6D0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC494
    li r0, 0x1
    b @L_801BC494
    li r0, 0x2
    b @L_801BC494
    li r0, 0x3
    b @L_801BC494
    li r0, 0x4
    b @L_801BC494
    li r0, 0x5
    b @L_801BC494
    li r0, 0x6
    b @L_801BC494
    li r0, 0x7
    b @L_801BC494
@L_801BC480:
    addi r3, r2, -22512
    li r4, 0x807
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC494:
    stw r0, 0xc(r31)
    lwz r0, 0xc(r31)
    cmplwi r0, 0x7
    bgt @L_801BC4FC
    lis r3, jumptable_8036D6B0@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D6B0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x40
    b @L_801BC514
    li r0, 0x43
    b @L_801BC514
    li r0, 0x46
    b @L_801BC514
    li r0, 0x49
    b @L_801BC514
    li r0, 0x4c
    b @L_801BC514
    li r0, 0x4f
    b @L_801BC514
    li r0, 0x52
    b @L_801BC514
    li r0, 0x55
    b @L_801BC514
@L_801BC4FC:
    lis r4, lbl_802756AC@ha
    addi r3, r2, -22512
    addi r5, r4, lbl_802756AC@l
    li r4, 0x258
    bl fn_80196D78
    li r0, 0x0
@L_801BC514:
    stw r0, 0x14(r31)
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmpwi r0, 0x4
    beq @L_801BC5CC
    bge @L_801BC538
    cmpwi r0, 0x1
    bge @L_801BC540
    b @L_801BC5CC
@L_801BC538:
    cmpwi r0, 0x7
    bge @L_801BC5CC
@L_801BC540:
    mr r0, r29
    addi r29, r29, 0x1
    cmplwi r0, 0x7
    bgt @L_801BC5A8
    lis r3, jumptable_8036D690@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D690@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC5BC
    li r0, 0x1
    b @L_801BC5BC
    li r0, 0x2
    b @L_801BC5BC
    li r0, 0x3
    b @L_801BC5BC
    li r0, 0x4
    b @L_801BC5BC
    li r0, 0x5
    b @L_801BC5BC
    li r0, 0x6
    b @L_801BC5BC
    li r0, 0x7
    b @L_801BC5BC
@L_801BC5A8:
    addi r3, r2, -22512
    li r4, 0x794
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC5BC:
    stw r0, 0xa4(r31)
    b @L_801BC5CC
@L_801BC5C4:
    li r0, 0xff
    stw r0, 0xc(r31)
@L_801BC5CC:
    lwz r31, 0x8(r31)
@L_801BC5D0:
    cmplwi r31, 0x0
    bne @L_801BC3CC
    mr r31, r25
    b @L_801BC68C
@L_801BC5E0:
    lwz r0, 0xc(r31)
    cmpwi r0, 0xff
    beq @L_801BC688
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    bne @L_801BC688
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 28, 31
    beq @L_801BC608
    b @L_801BC688
@L_801BC608:
    mr r0, r29
    addi r29, r29, 0x1
    cmplwi r0, 0x7
    bgt @L_801BC670
    lis r3, jumptable_8036D670@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D670@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC684
    li r0, 0x1
    b @L_801BC684
    li r0, 0x2
    b @L_801BC684
    li r0, 0x3
    b @L_801BC684
    li r0, 0x4
    b @L_801BC684
    li r0, 0x5
    b @L_801BC684
    li r0, 0x6
    b @L_801BC684
    li r0, 0x7
    b @L_801BC684
@L_801BC670:
    addi r3, r2, -22512
    li r4, 0x794
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC684:
    stw r0, 0xa4(r31)
@L_801BC688:
    lwz r31, 0x8(r31)
@L_801BC68C:
    cmplwi r31, 0x0
    bne @L_801BC5E0
    cmplwi r27, 0x0
    beq @L_801BC7A0
    mr r0, r30
    addi r30, r30, 0x1
    cmplwi r0, 0x7
    bgt @L_801BC704
    lis r3, jumptable_8036D650@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D650@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC718
    li r0, 0x1
    b @L_801BC718
    li r0, 0x2
    b @L_801BC718
    li r0, 0x3
    b @L_801BC718
    li r0, 0x4
    b @L_801BC718
    li r0, 0x5
    b @L_801BC718
    li r0, 0x6
    b @L_801BC718
    li r0, 0x7
    b @L_801BC718
@L_801BC704:
    addi r3, r2, -22512
    li r4, 0x807
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC718:
    stw r0, 0xc(r27)
    li r0, 0x39
    cmplwi r29, 0x7
    stw r0, 0x14(r27)
    bgt @L_801BC784
    lis r3, jumptable_8036D630@ha
    rlwinm r0, r29, 2, 0, 29
    addi r3, r3, jumptable_8036D630@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC798
    li r0, 0x1
    b @L_801BC798
    li r0, 0x2
    b @L_801BC798
    li r0, 0x3
    b @L_801BC798
    li r0, 0x4
    b @L_801BC798
    li r0, 0x5
    b @L_801BC798
    li r0, 0x6
    b @L_801BC798
    li r0, 0x7
    b @L_801BC798
@L_801BC784:
    addi r3, r2, -22512
    li r4, 0x794
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC798:
    stw r0, 0xa4(r27)
    addi r29, r29, 0x2
@L_801BC7A0:
    cmplwi r26, 0x0
    beq @L_801BC8A4
    mr r0, r30
    cmplwi r0, 0x7
    bgt @L_801BC80C
    lis r3, jumptable_8036D610@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D610@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC820
    li r0, 0x1
    b @L_801BC820
    li r0, 0x2
    b @L_801BC820
    li r0, 0x3
    b @L_801BC820
    li r0, 0x4
    b @L_801BC820
    li r0, 0x5
    b @L_801BC820
    li r0, 0x6
    b @L_801BC820
    li r0, 0x7
    b @L_801BC820
@L_801BC80C:
    addi r3, r2, -22512
    li r4, 0x807
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC820:
    mr r4, r29
    stw r0, 0xc(r26)
    cmplwi r4, 0x7
    addi r29, r29, 0x1
    bgt @L_801BC88C
    lis r3, jumptable_8036D5F0@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8036D5F0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r0, 0x0
    b @L_801BC8A0
    li r0, 0x1
    b @L_801BC8A0
    li r0, 0x2
    b @L_801BC8A0
    li r0, 0x3
    b @L_801BC8A0
    li r0, 0x4
    b @L_801BC8A0
    li r0, 0x5
    b @L_801BC8A0
    li r0, 0x6
    b @L_801BC8A0
    li r0, 0x7
    b @L_801BC8A0
@L_801BC88C:
    addi r3, r2, -22512
    li r4, 0x794
    addi r5, r2, -22480
    bl fn_80196E10
    li r0, 0x0
@L_801BC8A0:
    stw r0, 0xa4(r26)
@L_801BC8A4:
    mr r3, r29
@L_801BC8A8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

