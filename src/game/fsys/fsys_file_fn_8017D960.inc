    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    li r0, 0x3e8
    stmw r18, 0x8(r1)
    mr r31, r3
    stw r0, 0x48(r3)
    lwz r0, 0x50(r3)
    cmpwi r0, 0x1
    bne @L_8017DA50
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x3
    bne @L_8017D9FC
    li r30, 0x0
    b @L_8017D9EC
    @L_8017D99C:
    lwz r28, 0x40(r31)
    cmplwi r28, 0x0
    beq @L_8017D9C8
    lwz r3, 0x18(r28)
    slwi r0, r30, 2
    add r24, r28, r3
    lwz r3, 0x0(r24)
    add r25, r28, r3
    lwzx r0, r25, r0
    add r26, r28, r0
    b @L_8017D9CC
    @L_8017D9C8:
    li r26, 0x0
    @L_8017D9CC:
    mr r29, r26
    lwz r0, 0x104(r31)
    lwz r3, 0x0(r26)
    cmplw r3, r0
    bne @L_8017D9E8
    stw r30, 0xf4(r31)
    b @L_8017DA48
    @L_8017D9E8:
    addi r30, r30, 0x1
    @L_8017D9EC:
    lwz r0, 0xc(r31)
    cmplw r30, r0
    blt @L_8017D99C
    b @L_8017DA48
    @L_8017D9FC:
    li r30, 0x0
    b @L_8017DA3C
    @L_8017DA04:
    lwz r27, 0x40(r31)
    cmplwi r27, 0x0
    beq @L_8017DA30
    lwz r3, 0x18(r27)
    slwi r0, r30, 2
    add r20, r27, r3
    lwz r3, 0x0(r20)
    add r21, r27, r3
    lwzx r0, r21, r0
    add r22, r27, r0
    b @L_8017DA34
    @L_8017DA30:
    li r22, 0x0
    @L_8017DA34:
    stw r30, 0xf4(r31)
    addi r30, r30, 0x1
    @L_8017DA3C:
    lwz r0, 0xc(r31)
    cmplw r30, r0
    blt @L_8017DA04
    @L_8017DA48:
    li r0, 0x2
    stw r0, 0x4c(r31)
    @L_8017DA50:
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    beq @L_8017DA80
    lwz r3, 0x68(r31)
    bl fn_80167E64
    li r0, 0x0
    lis r4, lbl_80453FEC@ha
    stw r0, 0x68(r31)
    addi r4, r4, lbl_80453FEC@l
    li r0, 0x0
    stw r0, 0x1c(r4)
    bl fn_8017D800
    @L_8017DA80:
    lwz r0, 0x134(r31)
    cmplwi r0, 0x0
    beq @L_8017DAA4
    lwz r12, 0x134(r31)
    lwz r3, 0x4c(r31)
    lwz r4, 0x138(r31)
    lwz r5, 0x13c(r31)
    mtctr r12
    bctrl
    @L_8017DAA4:
    lmw r18, 0x8(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
