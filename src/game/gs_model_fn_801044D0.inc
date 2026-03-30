    nofralloc
    cmpwi r3, 0x0
    bgt @L_801044E0
    li r5, 0x0
    b @L_80104510
@L_801044E0:
    lis r5, lbl_80404ACC@ha
    addi r5, r5, lbl_80404ACC@l
    lwz r5, 0xc(r5)
    b @L_80104504
@L_801044F0:
    lwz r0, 0x4(r5)
    cmpw r0, r3
    bne @L_80104500
    b @L_80104510
@L_80104500:
    lwz r5, 0x10(r5)
@L_80104504:
    cmplwi r5, 0x0
    bne @L_801044F0
    li r5, 0x0
@L_80104510:
    cmplwi r5, 0x0
    beq @L_80104528
    lhz r0, 0x0(r4)
    li r3, 0x1
    sth r0, 0x94(r5)
    blr
@L_80104528:
    li r3, 0x0
    blr
