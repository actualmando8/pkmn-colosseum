    nofralloc
    cmplwi r3, 0x0
    bne @L_801BE4A0
    lwz r3, lbl_8047B37C(r13)
    b @L_801BE4BC
@L_801BE4A0:
    lwz r3, 0x8(r3)
    b @L_801BE4BC
@L_801BE4A8:
    lwz r0, 0x4c(r3)
    rlwinm r0, r0, 0, 28, 31
    cmplw r0, r4
    beqlr
    lwz r3, 0x8(r3)
@L_801BE4BC:
    cmplwi r3, 0x0
    bne @L_801BE4A8
    li r3, 0x0
    blr

