    nofralloc
    lbz r0, 0x18(r3)
    lwz r4, 0x2c(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    mr r5, r4
    bne @L_801DF198
    lhz r0, 0x14(r3)
    cmplwi r0, 0x10
    ble @L_801DF198
    addi r5, r4, 0xd40
    lwz r0, 0xdd4(r4)
    cmpwi r0, 0x1
    bne @L_801DF198
    mr r5, r4
@L_801DF198:
    lwz r0, 0x4(r5)
    addi r3, r5, 0x8c
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DF1C8
@L_801DF1AC:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DF1C0
    lwz r3, 0x4(r3)
    blr
@L_801DF1C0:
    addi r3, r3, 0x8
    bdnz @L_801DF1AC
@L_801DF1C8:
    li r3, 0x0
    blr
