    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x44(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_801326BC
    li r0, 0x1
    stb r0, 0x45(r31)
@L_801326BC:
    li r3, 0xa
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801326D8
    li r0, 0x0
    stb r0, 0x45(r31)
@L_801326D8:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_80132734
    lbz r0, 0x45(r31)
    cmplwi r0, 0x0
    beq @L_8013271C
    li r0, 0x0
    stb r0, 0x45(r31)
    lwz r0, 0x30(r31)
    stw r0, 0x2c(r31)
    lbz r0, 0x44(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    bne @L_80132744
    li r3, 0x24
    bl fn_80166A28
    b @L_80132744
@L_8013271C:
    lwz r3, 0x30(r31)
    li r0, 0x1
    subi r3, r3, 0x3
    stw r3, 0x30(r31)
    stb r0, 0x46(r31)
    b @L_80132744
@L_80132734:
    lfs f0, 0x4(r31)
    stfs f0, 0xc(r31)
    lfs f0, 0x8(r31)
    stfs f0, 0x10(r31)
@L_80132744:
    li r3, 0x1
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
