    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006C044
    b @L_8006C0C4
    @L_8006C044:
    bl fn_80105624
    mr r31, r3
    lwz r3, 0x4(r30)
    bl fn_801022B8
    cmpwi r3, 0xa0e
    bge @L_8006C07C
    cmpwi r3, 0x9fc
    bge @L_8006C070
    cmpwi r3, 0x9f7
    bge @L_8006C090
    b @L_8006C0BC
    @L_8006C070:
    cmpwi r3, 0xa0c
    bge @L_8006C0A4
    b @L_8006C0BC
    @L_8006C07C:
    cmpwi r3, 0xe35
    bge @L_8006C0BC
    cmpwi r3, 0xe33
    bge @L_8006C0A4
    b @L_8006C0BC
    @L_8006C090:
    lhz r0, 0x4(r31)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006C0BC
    b @L_8006C0C4
    @L_8006C0A4:
    lhz r0, 0x4(r31)
    rlwinm r0, r0, 0, 26, 27
    cmpwi r0, 0x0
    beq @L_8006C0BC
    b @L_8006C0C4
    b @L_8006C0C4
    @L_8006C0BC:
    mr r3, r30
    bl fn_80102ED4
    @L_8006C0C4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
