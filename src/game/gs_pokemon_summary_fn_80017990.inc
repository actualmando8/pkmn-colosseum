    nofralloc
    lbz r0, 0x95(r3)
    lis r5, lbl_80266918@ha
    addi r6, r5, lbl_80266918@l
    lha r7, 0x6(r4)
    extsb r0, r0
    mulli r8, r0, 0x4c
    add r5, r6, r8
    lwz r0, 0x10(r5)
    cmpw r7, r0
    bne @L_800179FC
    lbzx r5, r6, r8
    li r0, 0xff
    stb r5, 0x64(r4)
    lbz r5, 0x95(r3)
    extsb r5, r5
    mulli r5, r5, 0x4c
    add r5, r6, r5
    lbz r5, 0x1(r5)
    stb r5, 0x65(r4)
    lbz r3, 0x95(r3)
    extsb r3, r3
    mulli r3, r3, 0x4c
    add r3, r6, r3
    lbz r3, 0x2(r3)
    stb r3, 0x66(r4)
    stb r0, 0x67(r4)
    b @L_80017A04
    @L_800179FC:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_80017A04:
    li r3, 0x0
    blr
