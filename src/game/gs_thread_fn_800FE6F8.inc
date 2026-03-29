    nofralloc
    subi r0, r3, 0x1
    lwz r4, lbl_8047AC7C(r13)
    mulli r3, r0, 0x18
    li r5, 0x1
    addi r0, r3, 0xd
    stbx r5, r4, r0
    blr
