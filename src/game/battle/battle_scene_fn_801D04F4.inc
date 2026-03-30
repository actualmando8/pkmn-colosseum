    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, -21580(r13)
    lwz r0, 0xc(r3)
    cmpwi r0, 0x30
    bne @L_801D0530
    lwz r3, 0x4(r3)
    bl fn_801D0090
    lwz r4, -21580(r13)
    rlwinm r0, r3, 0, 24, 31
    stw r0, 0x4(r4)
    lwz r3, -21580(r13)
    lwz r0, 0x10(r3)
    stw r0, 0xc(r3)
@L_801D0530:
    lwz r3, -21580(r13)
    lwz r0, 0xc(r3)
    cmpwi r0, 0x32
    beq @L_801D0548
    li r3, 0x0
    b @L_801D054C
@L_801D0548:
    lwz r3, 0x8(r3)
@L_801D054C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
