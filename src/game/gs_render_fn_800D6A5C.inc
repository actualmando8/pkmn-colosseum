    nofralloc
    lis r5, lbl_804001F0@ha
    addi r6, r5, lbl_804001F0@l
    lwz r5, 0xc(r6)
    lwz r0, 0x4(r6)
    add r3, r5, r3
    add r0, r0, r4
    stw r3, 0xc(r6)
    stw r0, 0x4(r6)
    blr
