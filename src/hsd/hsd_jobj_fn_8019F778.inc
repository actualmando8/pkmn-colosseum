    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8019F7DC
    cmplwi r31, 0x0
    bne @L_8019F7A8
    la r3, lbl_8047DB34(r2)
    li r4, 0x25d
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    @L_8019F7A8:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_8019F7C8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_8019F7C8
    li r3, 0x1
    @L_8019F7C8:
    cmpwi r3, 0x0
    beq @L_8019F7D4
    b @L_8019F7DC
    @L_8019F7D4:
    mr r3, r31
    bl fn_8019D620
    @L_8019F7DC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
