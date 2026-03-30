    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r3
    lis r3, lbl_80274AA0@ha
    stw r28, 0x10(r1)
    addi r31, r3, lbl_80274AA0@l
    bne @L_801A02E4
    li r3, 0x0
    b @L_801A051C
    @L_801A02E4:
    lwz r30, 0x10(r29)
    cmplwi r30, 0x0
    beq @L_801A030C
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_801A030C
    addi r5, r31, 0x1cc
    la r3, lbl_8047DB20(r2)
    li r4, 0x4cb
    bl fn_80196E10
    @L_801A030C:
    cmplwi r30, 0x0
    beq @L_801A031C
    mr r28, r30
    b @L_801A0320
    @L_801A031C:
    lwz r28, 0x8(r29)
    @L_801A0320:
    cmplwi r29, 0x0
    beq @L_801A0334
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    bne @L_801A033C
    @L_801A0334:
    li r3, 0x0
    b @L_801A0390
    @L_801A033C:
    lwz r3, 0xc(r29)
    lwz r0, 0x10(r3)
    cmplw r29, r0
    bne @L_801A0354
    li r3, 0x0
    b @L_801A0390
    @L_801A0354:
    lwz r3, 0xc(r29)
    lwz r3, 0x10(r3)
    b @L_801A0374
    @L_801A0360:
    lwz r0, 0x8(r3)
    cmplw r0, r29
    bne @L_801A0370
    b @L_801A0390
    @L_801A0370:
    lwz r3, 0x8(r3)
    @L_801A0374:
    cmplwi r3, 0x0
    bne @L_801A0360
    addi r5, r31, 0x88
    la r3, lbl_8047DB20(r2)
    li r4, 0x5f8
    bl fn_80196D78
    li r3, 0x0
    @L_801A0390:
    cmplwi r3, 0x0
    beq @L_801A03A0
    stw r28, 0x8(r3)
    b @L_801A03B4
    @L_801A03A0:
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    beq @L_801A03B4
    lwz r3, 0xc(r29)
    stw r28, 0x10(r3)
    @L_801A03B4:
    cmplwi r28, 0x0
    beq @L_801A03D4
    cmplw r28, r30
    bne @L_801A03D4
    lwz r0, 0x8(r29)
    stw r0, 0x8(r28)
    lwz r0, 0xc(r29)
    stw r0, 0xc(r28)
    @L_801A03D4:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0xc(r29)
    cmplwi r29, 0x0
    li r0, 0x0
    stw r3, 0x10(r29)
    stw r0, 0x8(r29)
    beq @L_801A0518
    lis r3, 0x1
    lhz r4, 0x4(r29)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A0418
    b @L_801A0430
    @L_801A0418:
    lhz r0, 0x4(r29)
    lhz r3, 0x4(r29)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r29)
    srwi r3, r4, 5
    @L_801A0430:
    cmpwi r3, 0x0
    beq @L_801A0518
    lhz r3, 0x6(r29)
    subic. r0, r3, 0x1
    bge @L_801A0478
    cmplwi r29, 0x0
    beq @L_801A0518
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_801A0518
    @L_801A0478:
    lhz r3, 0x6(r29)
    addi r0, r3, 0x1
    sth r0, 0x6(r29)
    lhz r0, 0x6(r29)
    cmplwi r0, 0x0
    bne @L_801A04A0
    addi r3, r31, 0x54
    addi r5, r31, 0x60
    li r4, 0x9e
    bl fn_80196E10
    @L_801A04A0:
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lhz r0, 0x6(r29)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A04C8
    b @L_801A04E0
    @L_801A04C8:
    lhz r3, 0x6(r29)
    subi r0, r3, 0x1
    sth r0, 0x6(r29)
    lhz r0, 0x6(r29)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_801A04E0:
    cmpwi r0, 0x0
    beq @L_801A0518
    cmplwi r29, 0x0
    beq @L_801A0518
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A0518:
    mr r3, r30
    @L_801A051C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
