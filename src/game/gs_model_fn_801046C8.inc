    nofralloc
    cmplwi r3, 0x0
    bne @L_801046D8
    li r3, 0x0
    blr
@L_801046D8:
    lwz r3, 0x1c(r3)
    rlwinm r4, r4, 0, 16, 31
    b @L_801046F4
@L_801046E4:
    lha r0, 0x6(r3)
    cmpw r0, r4
    beqlr
    lwz r3, 0x0(r3)
@L_801046F4:
    cmplwi r3, 0x0
    bne @L_801046E4
    li r3, 0x0
    blr
