    nofralloc
    lwz r6, lbl_8047ACB4(r13)
    lwz r0, lbl_8047ACB8(r13)
    lwz r7, lbl_8047ACB0(r13)
    add r0, r6, r0
    lwz r6, lbl_8047ACBC(r13)
    mulli r0, r0, 0x24
    add r7, r7, r0
    mtctr r6
    cmplwi r6, 0x0
    beq @L_800FF1B8
@L_800FF1A0:
    lwz r0, 0x8(r7)
    cmpwi r0, 0x0
    bne @L_800FF1B0
    b @L_800FF1BC
@L_800FF1B0:
    addi r7, r7, 0x24
    bdnz @L_800FF1A0
@L_800FF1B8:
    li r7, 0x0
@L_800FF1BC:
    cmplwi r7, 0x0
    beqlr
    li r6, 0x0
    li r0, 0x5
    stw r6, 0x0(r7)
    stw r6, 0x4(r7)
    stw r0, 0x8(r7)
    stw r6, 0xc(r7)
    stw r4, 0x10(r7)
    stb r3, 0x14(r7)
    stb r6, 0x15(r7)
    lwz r0, 0xc(r7)
    cmpwi r0, 0x0
    bne @L_800FF1FC
    stw r5, 0x18(r7)
    b @L_800FF208
@L_800FF1FC:
    stw r5, 0x18(r7)
    sth r6, 0x20(r7)
    stw r6, 0x1c(r7)
@L_800FF208:
    lwz r4, lbl_8047ACCC(r13)
    cmplwi r4, 0x0
    bne @L_800FF224
    stw r7, lbl_8047ACCC(r13)
    blr
    b @L_800FF224
@L_800FF220:
    mr r4, r5
@L_800FF224:
    lwz r5, 0x4(r4)
    cmplwi r5, 0x0
    beq @L_800FF240
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    blt @L_800FF220
@L_800FF240:
    cmplwi r5, 0x0
    bne @L_800FF26C
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    bge @L_800FF26C
    stw r4, 0x0(r7)
    li r0, 0x0
    stw r0, 0x4(r7)
    stw r7, 0x4(r4)
    blr
@L_800FF26C:
    lwz r3, 0x0(r4)
    cmplwi r3, 0x0
    beq @L_800FF27C
    stw r7, 0x4(r3)
@L_800FF27C:
    lwz r0, 0x0(r4)
    stw r0, 0x0(r7)
    stw r4, 0x4(r7)
    stw r7, 0x0(r4)
    lwz r0, lbl_8047ACCC(r13)
    cmplw r0, r4
    bnelr
    stw r7, lbl_8047ACCC(r13)
    blr

