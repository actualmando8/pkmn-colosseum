    nofralloc
    lis r5, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7DFC
    b @L_800F7E30
    @L_800F7DFC:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E0C
    b @L_800F7E30
    @L_800F7E0C:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E1C
    b @L_800F7E30
    @L_800F7E1C:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E2C
    b @L_800F7E30
    @L_800F7E2C:
    mr r5, r6
    @L_800F7E30:
    cmplwi r5, 0x0
    beqlr
    stw r4, 0x14(r5)
    blr
