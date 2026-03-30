    nofralloc
    lwz r4, lbl_8047B610(r13)
    li r5, 0x0
    lbz r0, 0xd(r4)
    lwz r3, 0x1(r4)
    lwz r6, 0x5(r4)
    lwz r4, 0x9(r4)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80222694
@L_80222678:
    lbz r0, 0x0(r4)
    add r0, r5, r0
    addi r5, r5, 0x1
    lbzx r0, r6, r0
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
    bdnz @L_80222678
@L_80222694:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xe
    stw r0, lbl_8047B610(r13)
    blr
