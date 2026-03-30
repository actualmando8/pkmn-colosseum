    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 0, 29, 31
    bl fn_8005D7F8
    cmplwi r3, 0x0
    beq @L_801034C8
    mr r4, r31
    bl fn_8005D798
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_801034C8
    bl fn_80166A28
@L_801034C8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
