    nofralloc
    lwz r0, lbl_80478B98(r13)
    clrlwi r3, r3, 16
    cmplw r3, r0
    ble @L_801363C0
    li r3, 0x0
    b @L_801363D0
    @L_801363C0:
    mulli r4, r3, 0x18
    lis r3, lbl_80363B88@ha
    addi r0, r3, lbl_80363B88@l
    add r3, r0, r4
    @L_801363D0:
    cmplwi r3, 0x0
    bne @L_801363E0
    li r3, 0x0
    blr
    @L_801363E0:
    lbz r3, 0x0(r3)
    blr
