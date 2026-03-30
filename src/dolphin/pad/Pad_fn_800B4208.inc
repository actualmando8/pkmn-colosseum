    nofralloc
    addi r5, r3, 0x8
    li r7, 0x20
    b @L_800B424C
    @L_800B4214:
    lbz r0, 0x0(r5)
    addi r5, r5, 0x1
    lbz r6, 0x0(r4)
    addi r4, r4, 0x1
    extsb r3, r0
    extsb r0, r6
    cmpw r3, r0
    beq @L_800B423C
    li r3, 0x0
    blr
    @L_800B423C:
    extsb. r0, r6
    bne @L_800B424C
    li r3, 0x1
    blr
    @L_800B424C:
    subic. r7, r7, 0x1
    bge @L_800B4214
    lbz r0, 0x0(r4)
    extsb. r0, r0
    bne @L_800B4268
    li r3, 0x1
    blr
    @L_800B4268:
    li r3, 0x0
    blr
