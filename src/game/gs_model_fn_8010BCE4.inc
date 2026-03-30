    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010BD2C
@L_8010BD10:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010BD20
    b @L_8010BD30
@L_8010BD20:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010BD10
@L_8010BD2C:
    li r4, -0x1
@L_8010BD30:
    cmpwi r4, 0x0
    bge @L_8010BD40
    li r3, -0x1
    b @L_8010BD5C
@L_8010BD40:
    rlwinm r0, r4, 4, 0, 27
    add r3, r6, r0
    lbz r0, 0x6(r3)
    subfic r0, r0, 0x2
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    extsb r3, r0
@L_8010BD5C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

