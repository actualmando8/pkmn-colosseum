    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    cmpwi r30, 0x0
    blt @L_801D1D94
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r30, r0
    blt @L_801D1D9C
@L_801D1D94:
    li r3, 0x0
    b @L_801D1E38
@L_801D1D9C:
    lhz r0, 0x400(r3)
    cmplwi r0, 0x200
    blt @L_801D1DB0
    li r3, 0x0
    b @L_801D1E38
@L_801D1DB0:
    rlwinm r4, r30, 0, 16, 31
    rlwinm r0, r0, 1, 0, 30
    sthx r4, r3, r0
    li r4, 0xa
    lhz r5, 0x400(r3)
    addi r0, r5, 0x1
    sth r0, 0x400(r3)
    li r3, 0x0
    bl fn_80129280
    cmpwi r30, 0x0
    blt @L_801D1DE4
    cmpwi r30, 0x200
    blt @L_801D1DEC
@L_801D1DE4:
    li r0, -0x1
    b @L_801D1E18
@L_801D1DEC:
    rlwinm r0, r30, 29, 0, 2
    rlwinm r4, r30, 1, 31, 31
    subf r0, r4, r0
    li r5, 0x1
    rlwinm r0, r0, 3, 0, 31
    add r0, r0, r4
    subfic r0, r0, 0x7
    slw r4, r5, r0
    srawi r0, r30, 3
    rlwinm r31, r4, 0, 24, 31
    addze r0, r0
@L_801D1E18:
    cmpwi r0, 0x0
    blt @L_801D1E34
    add r3, r3, r0
    lbz r0, 0x402(r3)
    andc r0, r0, r31
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x402(r3)
@L_801D1E34:
    li r3, 0x1
@L_801D1E38:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
