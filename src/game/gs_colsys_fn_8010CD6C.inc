    nofralloc
    lis r3, -32704
    lwzu r5, 19560(r3)
    cmplwi r5, 0x0
    beqlr
    lwz r0, 0x3704(r3)
    li r6, 0x0
    lwz r7, 0x0(r5)
    mulli r0, r0, 0xdc0
    add r3, r3, r0
    addi r8, r3, 0x4
    b @L_8010CDF4
@L_8010CD98:
    lwz r4, 0x0(r7)
    li r0, 0x0
    lwz r3, 0x4(r7)
    addi r6, r6, 0x1
    stw r4, 0x0(r8)
    stw r3, 0x4(r8)
    lwz r3, 0x8(r7)
    stw r3, 0x8(r8)
    lwz r4, 0xc(r7)
    lwz r3, 0x10(r7)
    stw r4, 0xc(r8)
    stw r3, 0x10(r8)
    lwz r3, 0x14(r7)
    stw r3, 0x14(r8)
    lwz r4, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r4, 0x18(r8)
    stw r3, 0x1c(r8)
    lwz r3, 0x20(r7)
    addi r7, r7, 0x40
    stw r3, 0x20(r8)
    sth r0, 0x24(r8)
    addi r8, r8, 0x28
@L_8010CDF4:
    lwz r0, 0x4(r5)
    cmplw r6, r0
    blt @L_8010CD98
    blr
