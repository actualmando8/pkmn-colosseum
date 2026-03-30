    nofralloc
    lwz r3, lbl_8047B610(r13)
    li r9, 0x0
    lbz r5, 0x9(r3)
    lwz r6, 0x1(r3)
    lwz r7, 0x5(r3)
    rlwinm r0, r5, 0, 24, 31
    lwz r8, 0xa(r3)
    b @L_80222820
@L_802227F4:
    lbz r4, 0x0(r6)
    lbz r3, 0x0(r7)
    cmplw r4, r3
    beq @L_80222814
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xe
    stw r0, lbl_8047B610(r13)
    b @L_8022282C
@L_80222814:
    addi r6, r6, 0x1
    addi r7, r7, 0x1
    addi r9, r9, 0x1
@L_80222820:
    rlwinm r3, r9, 0, 24, 31
    cmplw r3, r0
    blt @L_802227F4
@L_8022282C:
    rlwinm r3, r9, 0, 24, 31
    rlwinm r0, r5, 0, 24, 31
    cmplw r3, r0
    bnelr
    stw r8, lbl_8047B610(r13)
    blr
