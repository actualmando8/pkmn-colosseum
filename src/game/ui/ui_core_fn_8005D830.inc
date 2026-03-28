    nofralloc
    lwz r0, lbl_80478900(r13)
    cmplw r3, r0
    blt @L_8005D844
    li r3, 0x0
    blr
@L_8005D844:
    mulli r4, r3, 0xc
    lis r3, lbl_802E6428@ha
    addi r0, r3, lbl_802E6428@l
    add r3, r0, r4
    blr
