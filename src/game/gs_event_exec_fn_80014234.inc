    nofralloc
    lis r5, lbl_80266B58@ha
    lwz r7, 0x60(r3)
    addi r6, r5, lbl_80266B58@l
    li r8, 0x0
    lwz r5, 0x4(r7)
    lwz r0, 0x0(r6)
    cmpw r5, r0
    beq @L_80014294
    addi r6, r6, 0xc
    li r8, 0x1
    lwz r0, 0x0(r6)
    cmpw r5, r0
    beq @L_80014294
    addi r6, r6, 0xc
    li r8, 0x2
    lwz r0, 0x0(r6)
    cmpw r5, r0
    beq @L_80014294
    addi r6, r6, 0xc
    li r8, 0x3
    lwz r0, 0x0(r6)
    cmpw r5, r0
    beq @L_80014294
    li r8, 0x4
    @L_80014294:
    cmpwi r8, 0x4
    blt @L_800142A4
    li r3, 0x0
    blr
    @L_800142A4:
    lbz r0, 0x95(r3)
    extsb r5, r0
    cmpwi r5, 0x0
    blt @L_800142D0
    mulli r6, r8, 0xc
    lis r3, lbl_80266B58@ha
    addi r0, r3, lbl_80266B58@l
    add r3, r0, r6
    lwz r0, 0x8(r3)
    cmpw r5, r0
    blt @L_800142D8
    @L_800142D0:
    li r3, 0x0
    blr
    @L_800142D8:
    mulli r0, r5, 0xc
    lwz r5, 0x4(r3)
    lha r3, 0x6(r4)
    add r5, r5, r0
    lwz r0, 0x4(r5)
    cmpw r0, r3
    beq @L_80014300
    lwz r0, 0x8(r5)
    cmpw r0, r3
    bne @L_8001430C
    @L_80014300:
    li r0, 0xff
    stb r0, 0x67(r4)
    b @L_80014314
    @L_8001430C:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_80014314:
    li r3, 0x0
    blr
