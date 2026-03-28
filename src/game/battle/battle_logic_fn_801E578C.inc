    nofralloc
    lwz r4, -21108(r13)
    lis r3, -32697
    addi r5, r3, -10728
    addi r0, r4, 0x1f
    clrrwi r6, r0, 5
    stw r6, 0x0(r5)
    addi r3, r6, 0x80
    addi r0, r6, 0x100
    stw r3, 0x4(r5)
    addi r4, r6, 0x180
    addi r3, r6, 0x200
    stw r0, 0x8(r5)
    addi r0, r6, 0x280
    stw r4, 0xc(r5)
    stw r3, 0x10(r5)
    stw r0, 0x14(r5)
    blr
