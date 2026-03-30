    nofralloc
    lwz r0, lbl_80478BA0(r13)
    clrlwi r3, r3, 16
    cmplw r3, r0
    ble @L_80136400
    li r3, 0x0
    b @L_80136410
    @L_80136400:
    mulli r4, r3, 0xc
    lis r3, lbl_80363C00@ha
    addi r0, r3, lbl_80363C00@l
    add r3, r0, r4
    @L_80136410:
    cmplwi r3, 0x0
    bne @L_80136420
    li r3, 0x0
    blr
    @L_80136420:
    lhz r3, 0x4(r3)
    blr
