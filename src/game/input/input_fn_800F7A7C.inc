    nofralloc
    lis r5, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7A94
    b @L_800F7AC8
    @L_800F7A94:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7AA4
    b @L_800F7AC8
    @L_800F7AA4:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7AB4
    b @L_800F7AC8
    @L_800F7AB4:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7AC4
    b @L_800F7AC8
    @L_800F7AC4:
    mr r5, r6
    @L_800F7AC8:
    cmplwi r5, 0x0
    bne @L_800F7AD8
    li r3, 0x0
    blr
    @L_800F7AD8:
    cmpwi r4, 0x1
    bne @L_800F7AE8
    lbz r3, 0x58(r5)
    blr
    @L_800F7AE8:
    lbz r3, 0x26(r5)
    blr
