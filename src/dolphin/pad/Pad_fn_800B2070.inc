    nofralloc
    li r0, 0x0
    srawi r4, r4, 1
    sth r0, 0x0(r6)
    addze. r4, r4
    sth r0, 0x0(r5)
    ble @L_800B21F4
    srwi. r0, r4, 3
    mtctr r0
    beq @L_800B21C4
    @L_800B2094:
    lhz r7, 0x0(r5)
    lhz r0, 0x0(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x0(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0x2(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x2(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0x4(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x4(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0x6(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x6(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0x8(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x8(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0xa(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0xa(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0xc(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0xc(r3)
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    lhz r7, 0x0(r5)
    lhz r0, 0xe(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0xe(r3)
    addi r3, r3, 0x10
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    bdnz @L_800B2094
    andi. r4, r4, 0x7
    beq @L_800B21F4
    @L_800B21C4:
    mtctr r4
    @L_800B21C8:
    lhz r7, 0x0(r5)
    lhz r0, 0x0(r3)
    add r0, r7, r0
    sth r0, 0x0(r5)
    lhz r0, 0x0(r3)
    addi r3, r3, 0x2
    lhz r7, 0x0(r6)
    nor r0, r0, r0
    add r0, r7, r0
    sth r0, 0x0(r6)
    bdnz @L_800B21C8
    @L_800B21F4:
    lhz r0, 0x0(r5)
    cmplwi r0, 0xffff
    bne @L_800B2208
    li r0, 0x0
    sth r0, 0x0(r5)
    @L_800B2208:
    lhz r0, 0x0(r6)
    cmplwi r0, 0xffff
    bnelr
    li r0, 0x0
    sth r0, 0x0(r6)
    blr
