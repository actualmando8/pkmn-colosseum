    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    lwz r30, 0x60(r31)
    bl fn_80105624
    lhz r0, 0x4(r3)
    li r4, 0x0
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8002C0AC
    lbz r3, 0x94(r31)
    lbz r0, 0x95(r31)
    extsb r3, r3
    extsb r0, r0
    add r4, r3, r0
    cmpwi r4, 0x0
    blt @L_8002C070
    lwz r0, 0x8(r30)
    cmpw r4, r0
    blt @L_8002C078
    @L_8002C070:
    li r3, 0x0
    b @L_8002C084
    @L_8002C078:
    lwz r3, 0x4(r30)
    slwi r0, r4, 1
    lhzx r3, r3, r0
    @L_8002C084:
    lbz r0, 0x1d(r30)
    mr r4, r3
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8002C0AC
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002C0AC
    li r3, 0x0
    b @L_8002C0CC
    @L_8002C0AC:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_8002C0C0
    lwz r3, 0x0(r30)
    sth r4, 0x0(r3)
    @L_8002C0C0:
    mr r3, r31
    bl fn_80102ED4
    li r3, 0x0
    @L_8002C0CC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
