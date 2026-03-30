    nofralloc
    lwz r0, 0x14(r3)
    clrlwi r5, r4, 24
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beqlr
    lwz r4, 0x18(r3)
    b @L_800E93AC
    @L_800E9378:
    lwz r3, 0x8(r4)
    cmplwi r3, 0x0
    beq @L_800E93A8
    cmplwi r5, 0x0
    beq @L_800E939C
    lwz r0, 0x4(r3)
    oris r0, r0, 0x400
    stw r0, 0x4(r3)
    b @L_800E93A8
    @L_800E939C:
    lwz r0, 0x4(r3)
    rlwinm r0, r0, 0, 6, 4
    stw r0, 0x4(r3)
    @L_800E93A8:
    lwz r4, 0x4(r4)
    @L_800E93AC:
    cmplwi r4, 0x0
    bne @L_800E9378
    blr
