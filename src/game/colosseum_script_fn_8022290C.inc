    nofralloc
    lwz r5, lbl_8047B610(r13)
    lbz r3, 0x1(r5)
    addi r0, r5, 0xc
    lwz r6, 0x2(r5)
    lhz r4, 0x6(r5)
    cmpwi r3, 0x3
    lwz r5, 0x8(r5)
    stw r0, lbl_8047B610(r13)
    beq @L_802229A4
    bge @L_8022294C
    cmpwi r3, 0x1
    beq @L_80222974
    bge @L_8022298C
    cmpwi r3, 0x0
    bge @L_8022295C
    blr
@L_8022294C:
    cmpwi r3, 0x5
    beq @L_802229D4
    bgelr
    b @L_802229BC
@L_8022295C:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    cmplw r3, r0
    bnelr
    stw r5, lbl_8047B610(r13)
    blr
@L_80222974:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    cmplw r3, r0
    beqlr
    stw r5, lbl_8047B610(r13)
    blr
@L_8022298C:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    cmplw r3, r0
    blelr
    stw r5, lbl_8047B610(r13)
    blr
@L_802229A4:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    cmplw r3, r0
    bgelr
    stw r5, lbl_8047B610(r13)
    blr
@L_802229BC:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    and. r0, r3, r0
    beqlr
    stw r5, lbl_8047B610(r13)
    blr
@L_802229D4:
    lhz r3, 0x0(r6)
    rlwinm r0, r4, 0, 16, 31
    and. r0, r3, r0
    bnelr
    stw r5, lbl_8047B610(r13)
    blr
