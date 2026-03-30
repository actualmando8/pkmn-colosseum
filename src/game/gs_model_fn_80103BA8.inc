    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r3
    cmpwi r4, 0x3
    beq @L_80103BF4
    bge @L_80103BD8
    cmpwi r4, 0x1
    beq @L_80103BE4
    bge @L_80103BEC
    b @L_80103C04
@L_80103BD8:
    cmpwi r4, 0x5
    bge @L_80103C04
    b @L_80103BFC
@L_80103BE4:
    li r3, 0x1
    b @L_80103C08
@L_80103BEC:
    li r3, 0x2
    b @L_80103C08
@L_80103BF4:
    li r3, 0x4
    b @L_80103C08
@L_80103BFC:
    li r3, 0x8
    b @L_80103C08
@L_80103C04:
    li r3, 0x0
@L_80103C08:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80103C54
    bl fn_801054B8
    lwz r4, 0x0(r3)
    lwz r0, 0x4(r3)
    stw r4, 0x8(r1)
    stw r0, 0xc(r1)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x10(r1)
    stw r0, 0x14(r1)
    lwz r4, 0x10(r3)
    lwz r0, 0x14(r3)
    stw r4, 0x18(r1)
    stw r0, 0x1c(r1)
    lhz r0, 0x18(r3)
    sth r0, 0x20(r1)
    b @L_80103C64
@L_80103C54:
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x1a
    bl memset
@L_80103C64:
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r3, 0x8(r31)
    stw r0, 0xc(r31)
    lwz r3, 0x18(r1)
    lwz r0, 0x1c(r1)
    stw r3, 0x10(r31)
    stw r0, 0x14(r31)
    lhz r0, 0x20(r1)
    sth r0, 0x18(r31)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
