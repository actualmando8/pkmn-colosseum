    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpw r0, r3
    bne @L_800F7C40
    b @L_800F7C74
    @L_800F7C40:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7C50
    b @L_800F7C74
    @L_800F7C50:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7C60
    b @L_800F7C74
    @L_800F7C60:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7C70
    b @L_800F7C74
    @L_800F7C70:
    mr r4, r5
    @L_800F7C74:
    cmplwi r4, 0x0
    bne @L_800F7C84
    li r3, 0x2
    blr
    @L_800F7C84:
    lwz r3, 0x4(r4)
    blr
