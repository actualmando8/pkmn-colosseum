    nofralloc
    lis r3, lbl_80266918@ha
    li r6, 0x0
    addi r5, r3, lbl_80266918@l
    lha r3, 0x6(r4)
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    addi r5, r5, 0x4c
    li r6, 0x1
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    addi r5, r5, 0x4c
    li r6, 0x2
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    addi r5, r5, 0x4c
    li r6, 0x3
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    addi r5, r5, 0x4c
    li r6, 0x4
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    addi r5, r5, 0x4c
    li r6, 0x5
    lwz r0, 0xc(r5)
    cmpw r3, r0
    beq @L_80017814
    li r6, 0x6
    @L_80017814:
    cmpwi r6, 0x6
    blt @L_80017824
    li r3, 0x0
    blr
    @L_80017824:
    mulli r0, r6, 0x4c
    lis r3, lbl_80266918@ha
    addi r5, r3, lbl_80266918@l
    lbzx r3, r5, r0
    add r5, r5, r0
    lwz r0, 0x4(r5)
    stb r3, 0x64(r4)
    lbz r3, 0x1(r5)
    cmpwi r0, -0x1
    lbz r0, 0x2(r5)
    stb r3, 0x65(r4)
    stb r0, 0x66(r4)
    bne @L_80017860
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_80017860:
    li r3, 0x0
    blr
