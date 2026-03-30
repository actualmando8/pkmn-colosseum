    nofralloc
    lwz r6, lbl_80478C94(r13)
    lis r5, 0x3
    addi r5, r5, 0x43fd
    lwz r4, lbl_80478C94(r13)
    lwz r0, 0x0(r6)
    mullw r5, r0, r5
    addis r5, r5, 0x27
    subi r0, r5, 0x613d
    stw r0, 0x0(r4)
    lwz r4, lbl_80478C94(r13)
    lwz r0, 0x0(r4)
    rlwinm r0, r0, 16, 16, 31
    mullw r0, r3, r0
    srawi r0, r0, 16
    addze r3, r0
    blr
