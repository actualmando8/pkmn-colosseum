    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801022A4
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102294
    lbz r0, 0x0(r3)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x0(r3)
    b @L_801022A4
@L_80102294:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x0(r3)
@L_801022A4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
