    nofralloc
    lwz r5, lbl_8047ABE0(r13)
    lis r4, lbl_804018F0@ha
    addi r6, r4, lbl_804018F0@l
    li r0, 0x4
    subf r3, r5, r3
    li r4, 0x0
    extrwi r3, r3, 8, 15
    mtctr r0
    @L_800EEB54:
    lbz r0, 0x0(r6)
    cmplw r0, r3
    bne @L_800EEB64
    b @L_800EEBE4
    @L_800EEB64:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEB74
    b @L_800EEBE4
    @L_800EEB74:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEB84
    b @L_800EEBE4
    @L_800EEB84:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEB94
    b @L_800EEBE4
    @L_800EEB94:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEBA4
    b @L_800EEBE4
    @L_800EEBA4:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEBB4
    b @L_800EEBE4
    @L_800EEBB4:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEBC4
    b @L_800EEBE4
    @L_800EEBC4:
    lbzu r0, 0x8(r6)
    cmplw r0, r3
    bne @L_800EEBD4
    b @L_800EEBE4
    @L_800EEBD4:
    addi r6, r6, 0x8
    addi r4, r4, 0x7
    bdnz @L_800EEB54
    li r6, 0x0
    @L_800EEBE4:
    cmplwi r6, 0x0
    beqlr
    lbz r5, 0x1(r6)
    lis r3, 0x8000
    lbz r4, 0x0(r6)
    b @L_800EEC00
    @L_800EEBFC:
    srwi r3, r3, 1
    @L_800EEC00:
    clrlwi. r0, r4, 24
    subi r4, r4, 0x1
    bne @L_800EEBFC
    lwz r4, lbl_8047ABEC(r13)
    b @L_800EEC1C
    @L_800EEC14:
    andc r4, r4, r3
    srwi r3, r3, 1
    @L_800EEC1C:
    clrlwi. r0, r5, 24
    subi r5, r5, 0x1
    bne @L_800EEC14
    stw r4, lbl_8047ABEC(r13)
    li r0, 0xff
    stb r0, 0x0(r6)
    blr
