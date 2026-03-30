    nofralloc
    lis r3, 0xcc00
    li r0, 0x0
    stw r0, 0x6004(r3)
    lwz r3, Callback_8047A788(r13)
    stw r0, Callback_8047A788(r13)
    blr
