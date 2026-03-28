    nofralloc
    lwz r0, lbl_80478968(r13)
    cmplw r3, r0
    blt @L_8005D90C
    li r3, 0x0
    b @L_8005D91C
@L_8005D90C:
    mulli r5, r3, 0x1c
    lis r3, lbl_802EF0A8@ha
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r5
@L_8005D91C:
    cmplwi r3, 0x0
    beqlr
    lbz r0, 0x0(r3)
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0x0(r3)
    blr
