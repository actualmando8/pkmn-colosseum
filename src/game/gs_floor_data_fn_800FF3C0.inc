    nofralloc
    lwz r0, lbl_8047ACB4(r13)
    lwz r7, lbl_8047ACB0(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FF3EC
@L_800FF3D4:
    lwz r0, 0x8(r7)
    cmpwi r0, 0x0
    bne @L_800FF3E4
    b @L_800FF3F0
@L_800FF3E4:
    addi r7, r7, 0x24
    bdnz @L_800FF3D4
@L_800FF3EC:
    li r7, 0x0
@L_800FF3F0:
    cmplwi r7, 0x0
    beqlr
    li r6, 0x0
    li r0, 0x1
    stw r6, 0x0(r7)
    stw r6, 0x4(r7)
    stw r0, 0x8(r7)
    stw r6, 0xc(r7)
    stw r4, 0x10(r7)
    stb r3, 0x14(r7)
    stb r6, 0x15(r7)
    lwz r0, 0xc(r7)
    cmpwi r0, 0x0
    bne @L_800FF430
    stw r5, 0x18(r7)
    b @L_800FF43C
@L_800FF430:
    stw r5, 0x18(r7)
    sth r6, 0x20(r7)
    stw r6, 0x1c(r7)
@L_800FF43C:
    lwz r4, lbl_8047ACCC(r13)
    cmplwi r4, 0x0
    bne @L_800FF458
    stw r7, lbl_8047ACCC(r13)
    blr
    b @L_800FF458
@L_800FF454:
    mr r4, r5
@L_800FF458:
    lwz r5, 0x4(r4)
    cmplwi r5, 0x0
    beq @L_800FF474
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    blt @L_800FF454
@L_800FF474:
    cmplwi r5, 0x0
    bne @L_800FF4A0
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    bge @L_800FF4A0
    stw r4, 0x0(r7)
    li r0, 0x0
    stw r0, 0x4(r7)
    stw r7, 0x4(r4)
    blr
@L_800FF4A0:
    lwz r3, 0x0(r4)
    cmplwi r3, 0x0
    beq @L_800FF4B0
    stw r7, 0x4(r3)
@L_800FF4B0:
    lwz r0, 0x0(r4)
    stw r0, 0x0(r7)
    stw r4, 0x4(r7)
    stw r7, 0x0(r4)
    lwz r0, lbl_8047ACCC(r13)
    cmplw r0, r4
    bnelr
    stw r7, lbl_8047ACCC(r13)
    blr

