    nofralloc
    clrlwi. r0, r3, 31
    beq @L_800E8FAC
    addi r3, r3, 0x1
    @L_800E8FAC:
    clrlwi. r0, r4, 31
    beq @L_800E8FB8
    addi r4, r4, 0x1
    @L_800E8FB8:
    cmpwi r3, 0x2
    bltlr
    cmpwi r4, 0x2
    bltlr
    cmpwi r3, 0x280
    bgtlr
    cmpwi r4, 0x1e0
    ble @L_800E8FDC
    blr
    @L_800E8FDC:
    stw r3, lbl_8047AB90(r13)
    stw r4, lbl_8047AB8C(r13)
    blr
