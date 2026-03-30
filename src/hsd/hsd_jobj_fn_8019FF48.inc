    nofralloc
    cmplwi r3, 0x0
    beq @L_8019FF64
    lwz r0, 0x14(r3)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    bne @L_8019FF6C
    @L_8019FF64:
    li r3, 0x0
    blr
    @L_8019FF6C:
    lwz r3, 0x18(r3)
    blr
