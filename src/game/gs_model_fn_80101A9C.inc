    nofralloc
    lwz r0, lbl_80478B20(r13)
    cmplw r3, r0
    blt @L_80101AB0
    li r3, 0x0
    blr
@L_80101AB0:
    lis r4, lbl_80315690@ha
    rlwinm r0, r3, 3, 0, 28
    addi r3, r4, lbl_80315690@l
    lwzx r3, r3, r0
    blr

