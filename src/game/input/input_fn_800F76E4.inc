    nofralloc
    li r0, 0x0
    stw r0, 0x14(r3)
    lwz r4, lbl_80478B00(r13)
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800F7704
    stw r3, 0x8(r4)
    b @L_800F770C
    @L_800F7704:
    stw r3, 0x8(r4)
    stw r0, 0x14(r3)
    @L_800F770C:
    lbz r0, 0xa(r3)
    cmplwi r0, 0x0
    bnelr
    lwz r0, 0xc(r3)
    li r6, 0x0
    add r5, r3, r0
    b @L_800F7740
    @L_800F7728:
    lwz r4, 0x0(r5)
    addi r5, r5, 0x4
    addi r6, r6, 0x1
    lwzx r0, r3, r4
    add r0, r0, r3
    stwx r0, r3, r4
    @L_800F7740:
    lhz r0, 0x6(r3)
    cmpw r6, r0
    blt @L_800F7728
    li r0, 0x1
    stb r0, 0xa(r3)
    blr
