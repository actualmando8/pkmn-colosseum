    nofralloc
    lwz r6, lbl_80478B00(r13)
    clrlwi r0, r3, 16
    li r5, 0x0
    lhz r3, 0x0(r6)
    mtctr r3
    cmpwi r3, 0x0
    ble @L_800F7150
    @L_800F7124:
    lwz r3, 0xc(r6)
    add r4, r3, r5
    lbz r3, 0x4(r4)
    cmplwi r3, 0x0
    beq @L_800F7148
    lhz r3, 0x6(r4)
    cmplw r3, r0
    bne @L_800F7148
    b @L_800F7154
    @L_800F7148:
    addi r5, r5, 0x16c
    bdnz @L_800F7124
    @L_800F7150:
    li r4, 0x0
    @L_800F7154:
    cmplwi r4, 0x0
    bne @L_800F7164
    li r3, 0x0
    blr
    @L_800F7164:
    lwz r3, 0xc(r4)
    blr
