    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpw r0, r3
    bne @L_800F7BDC
    b @L_800F7C10
    @L_800F7BDC:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7BEC
    b @L_800F7C10
    @L_800F7BEC:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7BFC
    b @L_800F7C10
    @L_800F7BFC:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7C0C
    b @L_800F7C10
    @L_800F7C0C:
    mr r4, r5
    @L_800F7C10:
    cmplwi r4, 0x0
    bne @L_800F7C20
    li r3, 0x0
    blr
    @L_800F7C20:
    lhz r3, 0x24(r4)
    blr
