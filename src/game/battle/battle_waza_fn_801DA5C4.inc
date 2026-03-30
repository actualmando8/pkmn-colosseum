    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32698
    mr r5, r3
    stw r0, 0x14(r1)
    addi r3, r4, 31936
    stw r31, 0xc(r1)
    lhz r0, 0x4(r3)
    lwz r3, 0x0(r3)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DA680
@L_801DA5F4:
    lbz r0, 0x74(r3)
    cmplwi r0, 0x0
    beq @L_801DA678
    lwz r31, 0x6c(r3)
    cmplwi r31, 0x0
    beq @L_801DA678
    bne @L_801DA618
    li r3, 0x1
    b @L_801DA684
@L_801DA618:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x6
    bne @L_801DA650
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801DA648
    lbz r0, 0x15(r31)
    extsb r0, r0
    cmpwi r0, -0x1
    beq @L_801DA648
    li r3, 0x0
    b @L_801DA684
@L_801DA648:
    li r3, 0x1
    b @L_801DA684
@L_801DA650:
    lwz r3, 0x3c(r31)
    mr r4, r31
    bl fn_801DB3F8
    lwz r0, 0x0(r31)
    rlwinm r4, r3, 1, 31, 31
    srawi r5, r0, 31
    subfc r0, r3, r0
    adde r0, r5, r4
    rlwinm r3, r0, 0, 24, 31
    b @L_801DA684
@L_801DA678:
    addi r3, r3, 0x8c
    bdnz @L_801DA5F4
@L_801DA680:
    li r3, 0x1
@L_801DA684:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
