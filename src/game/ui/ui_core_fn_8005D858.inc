    nofralloc
    lwz r0, lbl_80478908(r13)
    cmplw r3, r0
    blt @L_8005D86C
    li r3, 0x0
    blr
@L_8005D86C:
    mulli r4, r3, 0x18
    lis r3, lbl_802E7CE8@ha
    addi r0, r3, lbl_802E7CE8@l
    add r3, r0, r4
    blr
