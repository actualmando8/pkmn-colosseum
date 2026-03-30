    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_801DA968
    li r3, 0x0
    b @L_801DA9A4
@L_801DA968:
    bl fn_801DD0C8
    cmplwi r3, 0x0
    beq @L_801DA9A0
    lbz r0, 0x14(r3)
    li r4, 0x0
    cmplwi r0, 0x0
    beq @L_801DA998
    lbz r0, 0x15(r3)
    extsb r0, r0
    cmpwi r0, -0x1
    beq @L_801DA998
    li r4, 0x1
@L_801DA998:
    rlwinm r3, r4, 0, 24, 31
    b @L_801DA9A4
@L_801DA9A0:
    li r3, 0x0
@L_801DA9A4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
