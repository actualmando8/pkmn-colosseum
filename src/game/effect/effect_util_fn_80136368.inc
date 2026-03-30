    nofralloc
    lwz r0, lbl_80478B98(r13)
    clrlwi r3, r3, 16
    cmplw r3, r0
    ble @L_80136380
    li r3, 0x0
    b @L_80136390
    @L_80136380:
    mulli r4, r3, 0x18
    lis r3, lbl_80363B88@ha
    addi r0, r3, lbl_80363B88@l
    add r3, r0, r4
    @L_80136390:
    cmplwi r3, 0x0
    bne @L_801363A0
    li r3, 0x0
    blr
    @L_801363A0:
    lwz r3, 0x8(r3)
    blr
