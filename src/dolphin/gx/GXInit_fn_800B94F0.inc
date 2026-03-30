    nofralloc
    cmpwi r3, 0x2
    beq @L_800B9510
    bge @L_800B9514
    cmpwi r3, 0x1
    bge @L_800B9508
    b @L_800B9514
@L_800B9508:
    li r3, 0x2
    b @L_800B9514
@L_800B9510:
    li r3, 0x1
@L_800B9514:
    lwz r4, -29632(r13)
    rlwinm r0, r3, 14, 0, 17
    lwz r3, 0x204(r4)
    rlwinm r3, r3, 0, 18, 15
    or r0, r3, r0
    stw r0, 0x204(r4)
    lwz r0, 0x4f4(r4)
    ori r0, r0, 0x4
    stw r0, 0x4f4(r4)
    blr
