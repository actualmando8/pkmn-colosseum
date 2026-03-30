    nofralloc
    lwz r5, lbl_8047AD4C(r13)
    li r3, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r4, r5
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B594
@L_8010B57C:
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8010B594
    addi r4, r4, 0x10
    addi r3, r3, 0x1
    bdnz @L_8010B57C
@L_8010B594:
    mtctr r3
    cmpwi r3, 0x0
    ble @L_8010B5BC
@L_8010B5A0:
    lbz r0, 0x6(r5)
    cmplwi r0, 0x1
    bne @L_8010B5B4
    li r3, 0x1
    blr
@L_8010B5B4:
    addi r5, r5, 0x10
    bdnz @L_8010B5A0
@L_8010B5BC:
    li r3, 0x0
    blr

