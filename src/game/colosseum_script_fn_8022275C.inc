    nofralloc
    lwz r4, lbl_8047B610(r13)
    li r9, 0x0
    li r8, 0x0
    lbz r3, 0x9(r4)
    lwz r5, 0x1(r4)
    lwz r6, 0x5(r4)
    lwz r7, 0xa(r4)
    b @L_802227A4
@L_8022277C:
    lbz r4, 0x0(r5)
    lbz r0, 0x0(r6)
    cmplw r4, r0
    bne @L_80222798
    rlwinm r4, r9, 0, 24, 31
    addi r0, r4, 0x1
    rlwinm r9, r0, 0, 24, 31
@L_80222798:
    addi r5, r5, 0x1
    addi r6, r6, 0x1
    addi r8, r8, 0x1
@L_802227A4:
    rlwinm r0, r8, 0, 24, 31
    cmplw r0, r3
    blt @L_8022277C
    rlwinm r0, r9, 0, 24, 31
    cmplw r0, r3
    beq @L_802227C4
    mr r0, r7
    b @L_802227CC
@L_802227C4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xe
@L_802227CC:
    stw r0, lbl_8047B610(r13)
    blr
