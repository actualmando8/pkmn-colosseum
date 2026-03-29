    nofralloc
    lwz r6, lbl_80478B08(r13)
    li r9, 0x0
    li r5, 0x0
    lhz r7, 0x4(r6)
    mtctr r7
    cmpwi r7, 0x0
    ble @L_800FC2F4
@L_800FC2C4:
    lwz r0, 0x24(r6)
    add r8, r0, r5
    lwz r0, 0x4(r8)
    cmplwi r0, 0x0
    beq @L_800FC2E8
    lhz r4, 0x0(r8)
    lhz r0, 0x0(r3)
    cmplw r4, r0
    beq @L_800FC2F4
@L_800FC2E8:
    addi r5, r5, 0x8
    addi r9, r9, 0x1
    bdnz @L_800FC2C4
@L_800FC2F4:
    cmpw r9, r7
    beq @L_800FC380
    lwz r5, 0x4(r8)
    addi r0, r3, 0x8
    b @L_800FC378
@L_800FC308:
    cmplw r5, r0
    bne @L_800FC374
    lwz r4, 0xc(r5)
    cmplwi r4, 0x0
    bne @L_800FC340
    lwz r0, 0x8(r5)
    cmplwi r0, 0x0
    bne @L_800FC340
    lis r4, 0x1
    li r0, 0x0
    subi r4, r4, 0x1
    sth r4, 0x0(r8)
    stw r0, 0x4(r8)
    b @L_800FC380
@L_800FC340:
    cmplwi r4, 0x0
    beq @L_800FC354
    lwz r0, 0x8(r5)
    stw r0, 0x8(r4)
    b @L_800FC35C
@L_800FC354:
    lwz r0, 0x8(r5)
    stw r0, 0x4(r8)
@L_800FC35C:
    lwz r4, 0x8(r5)
    cmplwi r4, 0x0
    beq @L_800FC380
    lwz r0, 0xc(r5)
    stw r0, 0xc(r4)
    b @L_800FC380
@L_800FC374:
    lwz r5, 0x8(r5)
@L_800FC378:
    cmplwi r5, 0x0
    bne @L_800FC308
@L_800FC380:
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800FC394
    add r3, r3, r0
    b fn_800FC2A8
@L_800FC394:
    li r3, 0x0
    blr
