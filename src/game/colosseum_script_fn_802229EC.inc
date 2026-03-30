    nofralloc
    lwz r5, lbl_8047B610(r13)
    lbz r3, 0x1(r5)
    addi r0, r5, 0xb
    lwz r6, 0x2(r5)
    lbz r4, 0x6(r5)
    cmpwi r3, 0x3
    lwz r5, 0x7(r5)
    stw r0, lbl_8047B610(r13)
    beq @L_80222A84
    bge @L_80222A2C
    cmpwi r3, 0x1
    beq @L_80222A54
    bge @L_80222A6C
    cmpwi r3, 0x0
    bge @L_80222A3C
    blr
@L_80222A2C:
    cmpwi r3, 0x5
    beq @L_80222AB4
    bgelr
    b @L_80222A9C
@L_80222A3C:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    cmplw r3, r0
    bnelr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222A54:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    cmplw r3, r0
    beqlr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222A6C:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    cmplw r3, r0
    blelr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222A84:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    cmplw r3, r0
    bgelr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222A9C:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    and. r0, r3, r0
    beqlr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222AB4:
    lbz r3, 0x0(r6)
    rlwinm r0, r4, 0, 24, 31
    and. r0, r3, r0
    bnelr
    stw r5, lbl_8047B610(r13)
    blr
