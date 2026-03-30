    nofralloc
    lwz r4, 0x2e0(r3)
    lwz r5, 0x2dc(r3)
    cmplwi r4, 0x0
    lwz r6, 0x2e4(r3)
    bne @L_800A14A0
    stw r6, 0x4(r5)
    b @L_800A14A4
    @L_800A14A0:
    stw r6, 0x2e4(r4)
    @L_800A14A4:
    cmplwi r6, 0x0
    bne @L_800A14B4
    stw r4, 0x0(r5)
    b @L_800A14B8
    @L_800A14B4:
    stw r4, 0x2e0(r6)
    @L_800A14B8:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_800A14E0
    lwz r0, 0x2d0(r3)
    li r4, 0x1
    lwz r5, RunQueueBits_8047A760(r13)
    subfic r0, r0, 0x1f
    slw r0, r4, r0
    andc r0, r5, r0
    stw r0, RunQueueBits_8047A760(r13)
    @L_800A14E0:
    li r0, 0x0
    stw r0, 0x2dc(r3)
    blr
