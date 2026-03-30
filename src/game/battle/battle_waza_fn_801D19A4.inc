    nofralloc
    cmpwi r3, 0x0
    blt @L_801D19BC
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D19C4
@L_801D19BC:
    li r3, 0x0
    b @L_801D19D0
@L_801D19C4:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D19D0:
    cmplwi r3, 0x0
    bne @L_801D19E4
    lis r3, 0x1
    subi r7, r3, 0x1
    b @L_801D19E8
@L_801D19E4:
    lhz r7, 0x2(r3)
@L_801D19E8:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D1A00
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D1A00:
    lwz r4, -31104(r13)
    li r3, 0x0
    lwz r5, -31100(r13)
    lwz r6, 0x0(r4)
    b @L_801D1A34
@L_801D1A14:
    rlwinm r0, r7, 0, 16, 31
    rlwinm r4, r0, 2, 0, 29
    addi r0, r4, 0x2
    lhzx r0, r5, r0
    cmplwi r0, 0x0
    beqlr
    addi r3, r3, 0x1
    addi r7, r7, 0x1
@L_801D1A34:
    rlwinm r0, r7, 0, 16, 31
    cmplw r0, r6
    blt @L_801D1A14
    blr
