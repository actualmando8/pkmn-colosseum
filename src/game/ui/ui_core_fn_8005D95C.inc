    nofralloc
    cmpwi r3, 0x0
    bge @L_8005D968
    li r3, 0x0
@L_8005D968:
    lwz r0, lbl_80478848(r13)
    cmplw r3, r0
    blt @L_8005D978
    li r3, 0x1
@L_8005D978:
    mulli r6, r3, 0x1c
    lis r3, lbl_802E2DB8@ha
    cmplwi r4, 0x0
    addi r0, r3, lbl_802E2DB8@l
    add r3, r0, r6
    beq @L_8005D998
    lha r0, 0x6(r3)
    sth r0, 0x0(r4)
@L_8005D998:
    cmplwi r5, 0x0
    beqlr
    lha r0, 0x8(r3)
    sth r0, 0x0(r5)
    blr
