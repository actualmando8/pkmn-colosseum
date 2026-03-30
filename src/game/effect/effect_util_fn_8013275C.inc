    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x44(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80132788
    li r0, 0x1
    stb r0, 0x45(r31)
    @L_80132788:
    li r3, 0xa
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_801327A4
    li r0, 0x0
    stb r0, 0x45(r31)
    @L_801327A4:
    lbz r0, 0x45(r31)
    cmplwi r0, 0x0
    beq @L_801327BC
    li r0, 0x0
    stb r0, 0x45(r31)
    b @L_801327C8
    @L_801327BC:
    lwz r3, 0x30(r31)
    subi r0, r3, 0x3
    stw r0, 0x30(r31)
    @L_801327C8:
    li r3, 0x1
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
