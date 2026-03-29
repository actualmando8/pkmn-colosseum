    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lbz r0, 0x95(r3)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    lwz r4, lbl_8047A2E8(r13)
    srwi r0, r3, 16
    sth r0, 0x8(r1)
    cmpwi r4, 0x0
    blt @L_80016FBC
    lbz r3, 0x8(r1)
    lwz r0, lbl_8047A2C8(r13)
    extsb r3, r3
    subf r3, r3, r4
    cmpwi r0, 0x0
    mulli r3, r3, 0x1f
    addi r3, r3, 0x95
    beq @L_80016F94
    lfs f0, lbl_8047A2D0(r13)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    subf r3, r0, r3
    @L_80016F94:
    lha r0, 0x56(r31)
    add r0, r3, r0
    cmpwi r0, 0x95
    blt @L_80016FAC
    cmpwi r3, 0x18d
    blt @L_80016FB4
    @L_80016FAC:
    li r4, 0x0
    b @L_80017004
    @L_80016FB4:
    li r4, 0xff
    b @L_80017004
    @L_80016FBC:
    lbz r3, 0x9(r1)
    lwz r0, lbl_8047A2C8(r13)
    extsb r3, r3
    mulli r3, r3, 0x1f
    cmpwi r0, 0x0
    addi r3, r3, 0x95
    bne @L_80016FEC
    lfs f0, lbl_8047A2D0(r13)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r3, r3, r0
    @L_80016FEC:
    lwz r0, lbl_8047A2D8(r13)
    cmpwi r0, -0x1
    bne @L_80017000
    li r4, 0x72
    b @L_80017004
    @L_80017000:
    li r4, 0xff
    @L_80017004:
    extsh r0, r3
    li r3, 0x0
    sth r0, 0x52(r31)
    stb r4, 0x67(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
