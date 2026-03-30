    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, -21580(r13)
    lwz r0, 0xc(r4)
    cmpwi r0, 0x31
    bne @L_801D04AC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D04AC
    lwz r0, 0x10(r4)
    li r3, 0x3c
    stw r0, 0xc(r4)
    bl fn_800E0C04
    lwz r4, -21580(r13)
    stw r3, 0x34(r4)
    b @L_801D04C0
@L_801D04AC:
    li r3, 0xf
    li r0, 0x2b
    stw r3, 0x4(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
@L_801D04C0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
