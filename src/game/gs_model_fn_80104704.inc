    nofralloc
    cmpwi r3, 0x0
    bgt @L_80104714
    li r3, 0x0
    blr
@L_80104714:
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    lwz r4, 0xc(r4)
    b @L_8010473C
@L_80104724:
    lwz r0, 0x4(r4)
    cmpw r0, r3
    bne @L_80104738
    mr r3, r4
    blr
@L_80104738:
    lwz r4, 0x10(r4)
@L_8010473C:
    cmplwi r4, 0x0
    bne @L_80104724
    li r3, 0x0
    blr
