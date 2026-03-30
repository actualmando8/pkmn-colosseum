    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpw r0, r3
    bne @L_800F7F10
    b @L_800F7F44
    @L_800F7F10:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7F20
    b @L_800F7F44
    @L_800F7F20:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7F30
    b @L_800F7F44
    @L_800F7F30:
    lwzu r0, 0x6c(r4)
    cmpw r0, r3
    bne @L_800F7F40
    b @L_800F7F44
    @L_800F7F40:
    mr r4, r5
    @L_800F7F44:
    cmplwi r4, 0x0
    bne @L_800F7F54
    li r3, 0x0
    blr
    @L_800F7F54:
    lwz r0, 0xc(r4)
    cntlzw r0, r0
    extrwi r3, r0, 8, 19
    blr
