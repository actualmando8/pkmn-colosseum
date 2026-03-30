    nofralloc
    lis r5, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7E58
    b @L_800F7E8C
    @L_800F7E58:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E68
    b @L_800F7E8C
    @L_800F7E68:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E78
    b @L_800F7E8C
    @L_800F7E78:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7E88
    b @L_800F7E8C
    @L_800F7E88:
    mr r5, r6
    @L_800F7E8C:
    cmplwi r5, 0x0
    beqlr
    stb r4, 0x10(r5)
    blr
