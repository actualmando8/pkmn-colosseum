    nofralloc
    lwz r0, lbl_80478968(r13)
    cmplw r3, r0
    blt @L_8005D948
    li r3, 0x0
    blr
@L_8005D948:
    mulli r4, r3, 0x1c
    lis r3, lbl_802EF0A8@ha
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r4
    blr
