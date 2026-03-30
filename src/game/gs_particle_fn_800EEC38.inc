    nofralloc
    lbz r0, lbl_8047ABE8(r13)
    cmplwi r0, 0x1
    bne @L_800EEC4C
    li r3, 0x0
    blr
    @L_800EEC4C:
    clrlwi. r0, r3, 24
    beq @L_800EEC5C
    cmplwi r0, 0x20
    ble @L_800EEC64
    @L_800EEC5C:
    li r3, 0x0
    blr
    @L_800EEC64:
    lwz r9, lbl_8047ABEC(r13)
    li r10, 0x0
    b @L_800EEDE4
    @L_800EEC70:
    mr r8, r3
    mr r7, r10
    li r6, 0x0
    lis r5, 0x8000
    b @L_800EEC88
    @L_800EEC84:
    srwi r5, r5, 1
    @L_800EEC88:
    clrlwi. r0, r7, 24
    subi r7, r7, 0x1
    bne @L_800EEC84
    b @L_800EECBC
    @L_800EEC98:
    cmplwi r5, 0x0
    slwi r6, r6, 1
    beq @L_800EECB8
    and. r0, r9, r5
    beq @L_800EECB0
    ori r6, r6, 0x1
    @L_800EECB0:
    srwi r5, r5, 1
    b @L_800EECBC
    @L_800EECB8:
    ori r6, r6, 0x1
    @L_800EECBC:
    clrlwi. r0, r8, 24
    subi r8, r8, 0x1
    bne @L_800EEC98
    cmplwi r6, 0x0
    bne @L_800EEDE0
    lis r5, lbl_804018F0@ha
    li r0, 0x4
    addi r7, r5, lbl_804018F0@l
    li r5, 0x0
    mtctr r0
    @L_800EECE4:
    lbz r0, 0x0(r7)
    cmplwi r0, 0xff
    bne @L_800EECF4
    b @L_800EED74
    @L_800EECF4:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED04
    b @L_800EED74
    @L_800EED04:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED14
    b @L_800EED74
    @L_800EED14:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED24
    b @L_800EED74
    @L_800EED24:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED34
    b @L_800EED74
    @L_800EED34:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED44
    b @L_800EED74
    @L_800EED44:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED54
    b @L_800EED74
    @L_800EED54:
    lbzu r0, 0x8(r7)
    cmplwi r0, 0xff
    bne @L_800EED64
    b @L_800EED74
    @L_800EED64:
    addi r7, r7, 0x8
    addi r5, r5, 0x7
    bdnz @L_800EECE4
    li r7, 0x0
    @L_800EED74:
    cmplwi r7, 0x0
    bne @L_800EED84
    li r3, 0x0
    blr
    @L_800EED84:
    stb r10, 0x0(r7)
    mr r6, r10
    lis r5, 0x8000
    stb r3, 0x1(r7)
    stw r4, 0x4(r7)
    b @L_800EEDA0
    @L_800EED9C:
    srwi r5, r5, 1
    @L_800EEDA0:
    clrlwi. r0, r6, 24
    subi r6, r6, 0x1
    bne @L_800EED9C
    lwz r4, lbl_8047ABEC(r13)
    b @L_800EEDBC
    @L_800EEDB4:
    or r4, r4, r5
    srwi r5, r5, 1
    @L_800EEDBC:
    clrlwi. r0, r3, 24
    subi r3, r3, 0x1
    bne @L_800EEDB4
    stw r4, lbl_8047ABEC(r13)
    lwz r3, lbl_8047ABE0(r13)
    lbz r0, 0x0(r7)
    slwi r0, r0, 9
    add r3, r3, r0
    blr
    @L_800EEDE0:
    addi r10, r10, 0x1
    @L_800EEDE4:
    clrlwi r0, r10, 24
    cmplwi r0, 0x20
    blt @L_800EEC70
    li r3, 0x0
    blr
