    nofralloc
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x1(r3)
    lwz r5, 0x5(r3)
    lbz r0, 0x9(r3)
    mr r3, r4
    mr r4, r5
    mtctr r0
    cmpwi r0, 0x0
    ble @L_802226DC
@L_802226C8:
    lbz r0, 0x0(r4)
    addi r4, r4, 0x1
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
    bdnz @L_802226C8
@L_802226DC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    blr
