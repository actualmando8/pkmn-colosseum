    nofralloc
    lwz r5, lbl_80478C94(r13)
    lis r4, 0x3
    addi r4, r4, 0x43fd
    lwz r3, lbl_80478C94(r13)
    lwz r0, 0x0(r5)
    mullw r4, r0, r4
    addis r4, r4, 0x27
    subi r0, r4, 0x613d
    stw r0, 0x0(r3)
    lwz r3, lbl_80478C94(r13)
    lwz r0, 0x0(r3)
    rlwinm r3, r0, 16, 16, 31
    blr
