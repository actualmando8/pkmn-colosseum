    nofralloc
    lwz r0, lbl_80478B20(r13)
    cmplw r3, r0
    blt @L_80101A84
    li r3, 0x0
    blr
@L_80101A84:
    lis r4, lbl_80315690@ha
    rlwinm r0, r3, 3, 0, 28
    addi r3, r4, lbl_80315690@l
    add r3, r3, r0
    lwz r3, 0x4(r3)
    blr

