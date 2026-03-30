    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpw r0, r3
    bne @L_800F7B08
    b @L_800F7B3C
    @L_800F7B08:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7B18
    b @L_800F7B3C
    @L_800F7B18:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7B28
    b @L_800F7B3C
    @L_800F7B28:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7B38
    b @L_800F7B3C
    @L_800F7B38:
    mr r4, r5
    @L_800F7B3C:
    cmplwi r4, 0x0
    bne @L_800F7B4C
    li r3, 0x0
    blr
    @L_800F7B4C:
    lhz r3, 0x24(r4)
    lwz r0, 0x30(r4)
    xor r3, r3, r0
    blr
