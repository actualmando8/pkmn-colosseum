    nofralloc
    cmpwi r3, 0x0
    blt @L_80077D9C
    lwz r0, lbl_80478928(r13)
    cmplw r0, r3
    bgt @L_80077DA4
@L_80077D9C:
    li r3, 0x0
    blr
@L_80077DA4:
    lis r4, lbl_802EE458@ha
    rlwinm r0, r3, 1, 0, 30
    addi r3, r4, lbl_802EE458@l
    lhzx r3, r3, r0
    blr
