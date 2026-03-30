    nofralloc
    rlwinm r3, r3, 0, 24, 31
    lwz r0, -21000(r13)
    cmplwi r3, 0x1
    ori r3, r0, 0x80
    bne @L_80211828
    rlwinm r3, r0, 0, 25, 23
@L_80211828:
    stw r3, -21000(r13)
    blr
