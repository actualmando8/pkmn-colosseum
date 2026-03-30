    nofralloc
    stwu r1, -0x10(r1)
    lhz r0, lbl_8047E718(r13)
    cmpwi r3, 0x0
    sth r0, 0x8(r1)
    bgt @L_8010454C
    li r4, 0x0
    b @L_8010457C
@L_8010454C:
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    lwz r4, 0xc(r4)
    b @L_80104570
@L_8010455C:
    lwz r0, 0x4(r4)
    cmpw r0, r3
    bne @L_8010456C
    b @L_8010457C
@L_8010456C:
    lwz r4, 0x10(r4)
@L_80104570:
    cmplwi r4, 0x0
    bne @L_8010455C
    li r4, 0x0
@L_8010457C:
    cmplwi r4, 0x0
    beq @L_80104590
    lhz r0, 0x94(r4)
    sth r0, 0xc(r1)
    b @L_80104598
@L_80104590:
    lhz r0, 0x8(r1)
    sth r0, 0xc(r1)
@L_80104598:
    lhz r0, 0xc(r1)
    rlwinm r3, r0, 16, 0, 15
    addi r1, r1, 0x10
    blr
