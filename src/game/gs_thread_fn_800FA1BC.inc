    nofralloc
    stwu r1, -0x20(r1)
    lwz r6, lbl_80478B08(r13)
    li r5, 0x0
    lhz r0, 0x4(r6)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800FA278
@L_800FA1D8:
    lwz r4, 0x24(r6)
    lhz r0, 0x20(r3)
    add r7, r4, r5
    lhz r4, 0x0(r7)
    cmplw r4, r0
    bne @L_800FA270
    lbz r0, 0x2(r7)
    stb r0, 0x22(r3)
    lbz r0, 0x3(r7)
    stb r0, 0x23(r3)
    lhz r0, 0x20(r3)
    cmplwi r0, 0x0
    bne @L_800FA218
    li r0, 0xb
    stb r0, 0x42(r3)
    b @L_800FA278
@L_800FA218:
    cmplwi r0, 0x1
    beq @L_800FA224
    bne @L_800FA230
@L_800FA224:
    li r0, 0x6
    stb r0, 0x42(r3)
    b @L_800FA278
@L_800FA230:
    lbz r4, 0x23(r3)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r4, 0xc(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x8(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    extsb r0, r0
    stb r0, 0x42(r3)
    b @L_800FA278
@L_800FA270:
    addi r5, r5, 0x8
    bdnz @L_800FA1D8
@L_800FA278:
    addi r1, r1, 0x20
    blr
