    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    lis r31, lbl_80404ACC@ha
    li r26, 0x0
    addi r31, r31, lbl_80404ACC@l
    addi r30, r13, -23324
    addi r29, r13, -23320
@L_8010391C:
    lbz r5, 0x0(r30)
    li r0, 0x0
    sth r0, 0x8(r1)
    mr r3, r26
    addi r28, r31, 0x2a
    addi r4, r1, 0x8
    stb r5, 0x0(r29)
    bl fn_80103614
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80103978
    mr r3, r26
    addi r4, r1, 0x8
    bl fn_801034DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80103970
    li r0, 0x0
    li r3, 0x0
    sth r0, 0x8(r1)
    b @L_8010397C
@L_80103970:
    li r3, 0x2
    b @L_8010397C
@L_80103978:
    li r3, 0x1
@L_8010397C:
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_801039E8
    bge @L_80103998
    cmpwi r0, 0x0
    bge @L_801039A4
    b @L_80103A28
@L_80103998:
    cmpwi r0, 0x3
    bge @L_80103A28
    b @L_801039F4
@L_801039A4:
    lbz r0, 0x0(r29)
    cmpwi r0, 0x2
    bge @L_801039B8
    cmpwi r0, 0x0
    b @L_801039C4
@L_801039B8:
    cmpwi r0, 0x4
    bge @L_801039C4
    b @L_801039D0
@L_801039C4:
    li r0, 0x0
    stb r0, 0x0(r30)
    b @L_80103A2C
@L_801039D0:
    lis r3, 0x1
    li r4, 0x3
    addi r0, r3, -0x8000
    stb r4, 0x0(r30)
    sth r0, 0x8(r1)
    b @L_80103A2C
@L_801039E8:
    li r0, 0x1
    stb r0, 0x0(r30)
    b @L_80103A2C
@L_801039F4:
    lbz r0, 0x0(r29)
    cmpwi r0, 0x1
    beq @L_80103A1C
    bge @L_80103A0C
    cmpwi r0, 0x0
    b @L_80103A10
@L_80103A0C:
    cmpwi r0, 0x4
@L_80103A10:
    li r0, 0x2
    stb r0, 0x0(r30)
    b @L_80103A2C
@L_80103A1C:
    li r0, 0x2
    stb r0, 0x0(r30)
    b @L_80103A2C
@L_80103A28:
    stb r3, 0x0(r30)
@L_80103A2C:
    lhz r3, 0x0(r28)
    li r24, 0x0
    lhz r0, 0x8(r1)
    li r23, 0x0
    sth r3, 0x2(r28)
    li r27, 0x0
    lhz r3, 0x2(r28)
    xori r3, r3, 0xffff
    and r0, r3, r0
    rlwinm r25, r0, 0, 16, 31
@L_80103A54:
    li r0, 0x1
    slw r0, r0, r27
    rlwinm r22, r0, 0, 16, 31
    and r0, r25, r22
    cmpwi r0, 0x0
    beq @L_80103A80
    addi r0, r27, 0xa
    li r3, 0xf
    stbx r3, r28, r0
    or r24, r24, r22
    b @L_80103ADC
@L_80103A80:
    lhz r0, 0x8(r1)
    and r0, r0, r22
    cmpwi r0, 0x0
    beq @L_80103ADC
    bl fn_800D3088
    addi r4, r27, 0xa
    lbzx r0, r28, r4
    subf r0, r3, r0
    extsb r0, r0
    stbx r0, r28, r4
    lbzx r0, r28, r4
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_80103ACC
    li r0, 0x5
    or r24, r24, r22
    stbx r0, r28, r4
    or r23, r23, r22
    b @L_80103ADC
@L_80103ACC:
    lhz r0, 0x8(r28)
    and r0, r22, r0
    or r0, r23, r0
    rlwinm r23, r0, 0, 16, 31
@L_80103ADC:
    addi r27, r27, 0x1
    cmpwi r27, 0x10
    blt @L_80103A54
    rlwinm r0, r25, 0, 28, 31
    cmpwi r0, 0x0
    beq @L_80103B08
    li r0, 0xf
    stb r0, 0xa(r28)
    stb r0, 0xb(r28)
    stb r0, 0xc(r28)
    stb r0, 0xd(r28)
@L_80103B08:
    lhz r0, 0x8(r1)
    addi r31, r31, 0x1a
    addi r30, r30, 0x1
    addi r29, r29, 0x1
    sth r0, 0x0(r28)
    addi r26, r26, 0x1
    sth r25, 0x4(r28)
    sth r24, 0x6(r28)
    sth r23, 0x8(r28)
    cmpwi r26, 0x4
    blt @L_8010391C
    bl fn_801046B8
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80103B4C
    li r3, 0x1
    b @L_80103B50
@L_80103B4C:
    lbz r3, 0xb(r3)
@L_80103B50:
    bl fn_801054B8
    lis r5, lbl_80404ACC@ha
    lwz r4, 0x0(r3)
    lwz r0, 0x4(r3)
    addi r5, r5, lbl_80404ACC@l
    stw r4, 0x10(r5)
    stw r0, 0x14(r5)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x18(r5)
    stw r0, 0x1c(r5)
    lwz r4, 0x10(r3)
    lwz r0, 0x14(r3)
    stw r4, 0x20(r5)
    stw r0, 0x24(r5)
    lhz r0, 0x18(r3)
    sth r0, 0x28(r5)
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
