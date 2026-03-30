    nofralloc
    cmpwi r3, 0x0
    blt @L_801D174C
    lwz r5, -31112(r13)
    lwz r0, 0x0(r5)
    cmplw r3, r0
    blt @L_801D1754
@L_801D174C:
    li r5, 0x0
    b @L_801D1760
@L_801D1754:
    mulli r0, r3, 0x2c
    lwz r5, -31108(r13)
    add r5, r5, r0
@L_801D1760:
    cmplwi r5, 0x0
    bne @L_801D1774
    lis r5, 0x1
    subi r7, r5, 0x1
    b @L_801D1778
@L_801D1774:
    lhz r7, 0x2(r5)
@L_801D1778:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D1790
    lis r5, 0x1
    subi r6, r5, 0x1
    b @L_801D17D0
@L_801D1790:
    lwz r5, -31104(r13)
    li r6, 0x0
    lwz r8, -31100(r13)
    lwz r9, 0x0(r5)
    b @L_801D17C4
@L_801D17A4:
    rlwinm r0, r7, 0, 16, 31
    rlwinm r5, r0, 2, 0, 29
    addi r0, r5, 0x2
    lhzx r0, r8, r0
    cmplwi r0, 0x0
    beq @L_801D17D0
    addi r6, r6, 0x1
    addi r7, r7, 0x1
@L_801D17C4:
    rlwinm r0, r7, 0, 16, 31
    cmplw r0, r9
    blt @L_801D17A4
@L_801D17D0:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D17E4
    li r3, 0xff
    blr
@L_801D17E4:
    cmpw r4, r0
    blt @L_801D17F4
    li r3, 0xff
    blr
@L_801D17F4:
    cmpwi r3, 0x0
    blt @L_801D180C
    lwz r5, -31112(r13)
    lwz r0, 0x0(r5)
    cmplw r3, r0
    blt @L_801D1814
@L_801D180C:
    li r3, 0x0
    b @L_801D1820
@L_801D1814:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1820:
    cmplwi r3, 0x0
    bne @L_801D1834
    lis r3, 0x1
    subi r5, r3, 0x1
    b @L_801D1838
@L_801D1834:
    lhz r5, 0x2(r3)
@L_801D1838:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D184C
    li r3, 0xff
    blr
@L_801D184C:
    add r0, r5, r4
    lwz r3, -31100(r13)
    rlwinm r5, r0, 0, 16, 31
    rlwinm r0, r5, 2, 0, 29
    lbzx r3, r3, r0
    blr
