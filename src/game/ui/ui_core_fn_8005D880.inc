    nofralloc
    lwz r0, lbl_80478968(r13)
    cmplw r3, r0
    blt @L_8005D894
    li r3, 0x0
    b @L_8005D8A4
@L_8005D894:
    mulli r6, r3, 0x1c
    lis r3, lbl_802EF0A8@ha
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r6
@L_8005D8A4:
    cmplwi r3, 0x0
    beqlr
    sth r4, 0x2(r3)
    sth r5, 0x4(r3)
    blr
