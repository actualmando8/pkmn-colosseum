    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x95(r30)
    extsb r0, r0
    cmpwi r0, 0x6
    bge @L_8006FEC4
    mr r31, r0
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006FEC4
    mr r3, r31
    bl fn_8006B3C8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006FEC4
    li r3, 0x26
    bl fn_80166A28
    b @L_8006FECC
    @L_8006FEC4:
    mr r3, r30
    bl fn_80102ED4
    @L_8006FECC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
