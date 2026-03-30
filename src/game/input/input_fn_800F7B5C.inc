    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpw r0, r3
    bne @L_800F7B74
    b @L_800F7BA8
    @L_800F7B74:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7B84
    b @L_800F7BA8
    @L_800F7B84:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7B94
    b @L_800F7BA8
    @L_800F7B94:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7BA4
    b @L_800F7BA8
    @L_800F7BA4:
    mr r4, r5
    @L_800F7BA8:
    cmplwi r4, 0x0
    bne @L_800F7BB8
    li r3, 0x0
    blr
    @L_800F7BB8:
    lhz r0, 0x24(r4)
    nor r3, r0, r0
    blr
