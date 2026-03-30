    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801DA598
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_801DA598
    rlwinm. r0, r4, 0, 24, 31
    beq @L_801DA554
    lbz r0, 0x18(r31)
    li r4, 0x1
    ori r0, r0, 0x1
    stb r0, 0x18(r31)
    bl fn_800E4014
    lwz r3, 0x80(r31)
    cmplwi r3, 0x0
    beq @L_801DA53C
    li r4, 0x1
    bl fn_80118C88
@L_801DA53C:
    lwz r3, 0x84(r31)
    cmplwi r3, 0x0
    beq @L_801DA598
    li r4, 0x1
    bl fn_80118C88
    b @L_801DA598
@L_801DA554:
    lbz r4, 0x18(r31)
    rlwinm. r0, r4, 0, 31, 31
    beq @L_801DA568
    xori r0, r4, 0x1
    stb r0, 0x18(r31)
@L_801DA568:
    li r4, 0x0
    bl fn_800E4014
    lwz r3, 0x80(r31)
    cmplwi r3, 0x0
    beq @L_801DA584
    li r4, 0x0
    bl fn_80118C88
@L_801DA584:
    lwz r3, 0x84(r31)
    cmplwi r3, 0x0
    beq @L_801DA598
    li r4, 0x0
    bl fn_80118C88
@L_801DA598:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
