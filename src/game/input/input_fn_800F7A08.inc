    nofralloc
    lis r5, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7A20
    b @L_800F7A54
    @L_800F7A20:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7A30
    b @L_800F7A54
    @L_800F7A30:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7A40
    b @L_800F7A54
    @L_800F7A40:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7A50
    b @L_800F7A54
    @L_800F7A50:
    mr r5, r6
    @L_800F7A54:
    cmplwi r5, 0x0
    bne @L_800F7A64
    li r3, 0x0
    blr
    @L_800F7A64:
    cmpwi r4, 0x1
    bne @L_800F7A74
    lbz r3, 0x59(r5)
    blr
    @L_800F7A74:
    lbz r3, 0x27(r5)
    blr
