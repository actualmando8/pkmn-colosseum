    nofralloc
    lwz r4, lbl_8047B610(r13)
    lbz r3, 0x1(r4)
    addi r0, r4, 0xe
    lwz r5, 0x2(r4)
    lwz r6, 0x6(r4)
    cmpwi r3, 0x3
    lwz r4, 0xa(r4)
    stw r0, lbl_8047B610(r13)
    beq @L_802228D0
    bge @L_80222884
    cmpwi r3, 0x1
    beq @L_802228A8
    bge @L_802228BC
    cmpwi r3, 0x0
    bge @L_80222894
    blr
@L_80222884:
    cmpwi r3, 0x5
    beq @L_802228F8
    bgelr
    b @L_802228E4
@L_80222894:
    lwz r0, 0x0(r5)
    cmplw r0, r6
    bnelr
    stw r4, lbl_8047B610(r13)
    blr
@L_802228A8:
    lwz r0, 0x0(r5)
    cmplw r0, r6
    beqlr
    stw r4, lbl_8047B610(r13)
    blr
@L_802228BC:
    lwz r0, 0x0(r5)
    cmplw r0, r6
    blelr
    stw r4, lbl_8047B610(r13)
    blr
@L_802228D0:
    lwz r0, 0x0(r5)
    cmplw r0, r6
    bgelr
    stw r4, lbl_8047B610(r13)
    blr
@L_802228E4:
    lwz r0, 0x0(r5)
    and. r0, r0, r6
    beqlr
    stw r4, lbl_8047B610(r13)
    blr
@L_802228F8:
    lwz r0, 0x0(r5)
    and. r0, r0, r6
    bnelr
    stw r4, lbl_8047B610(r13)
    blr
