    nofralloc
    stwu r1, -0x20(r1)
    lhz r0, 0x94(r3)
    lis r5, lbl_802E4F68@ha
    lwz r7, 0x60(r3)
    addi r3, r5, lbl_802E4F68@l
    sth r0, 0x8(r1)
    li r5, 0x0
    lha r6, 0x6(r4)
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002BD68
    addi r3, r3, 0x8
    li r5, 0x1
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002BD68
    addi r3, r3, 0x8
    li r5, 0x2
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002BD68
    addi r3, r3, 0x8
    li r5, 0x3
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002BD68
    addi r3, r3, 0x8
    li r5, 0x4
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002BD68
    li r5, 0x5
    @L_8002BD68:
    cmpwi r5, 0x5
    blt @L_8002BD78
    li r6, 0x0
    b @L_8002BD88
    @L_8002BD78:
    lis r3, lbl_802E4F68@ha
    slwi r5, r5, 3
    addi r0, r3, lbl_802E4F68@l
    add r6, r0, r5
    @L_8002BD88:
    cmplwi r6, 0x0
    bne @L_8002BD98
    li r3, 0x0
    b @L_8002BE00
    @L_8002BD98:
    lbz r0, 0x9(r1)
    lwz r3, 0x14(r7)
    extsb r5, r0
    lha r6, 0x4(r6)
    lwz r0, 0x0(r3)
    mulli r3, r5, 0x1f
    cmpwi r0, 0x0
    add r5, r6, r3
    bne @L_8002BDD4
    lwz r3, 0xc(r7)
    lfs f0, 0x0(r3)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r5, r5, r0
    @L_8002BDD4:
    lwz r3, 0x0(r7)
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002BDEC
    li r6, 0x72
    b @L_8002BDF0
    @L_8002BDEC:
    li r6, 0xff
    @L_8002BDF0:
    extsh r0, r5
    li r3, 0x0
    sth r0, 0x52(r4)
    stb r6, 0x67(r4)
    @L_8002BE00:
    addi r1, r1, 0x20
    blr
