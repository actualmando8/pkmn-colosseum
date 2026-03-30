    nofralloc
    lis r5, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7EB4
    b @L_800F7EE8
    @L_800F7EB4:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7EC4
    b @L_800F7EE8
    @L_800F7EC4:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7ED4
    b @L_800F7EE8
    @L_800F7ED4:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7EE4
    b @L_800F7EE8
    @L_800F7EE4:
    mr r5, r6
    @L_800F7EE8:
    cmplwi r5, 0x0
    beqlr
    stw r4, 0x8(r5)
    blr
