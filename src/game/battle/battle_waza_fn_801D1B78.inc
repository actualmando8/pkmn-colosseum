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
    blt @L_801D1BAC
    cmpwi r30, 0x200
    blt @L_801D1BB4
@L_801D1BAC:
    li r0, -0x1
    b @L_801D1BE0
@L_801D1BB4:
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
@L_801D1BE0:
    cmpwi r0, 0x0
    bge @L_801D1BF0
    li r3, 0x0
    b @L_801D1C08
@L_801D1BF0:
    add r3, r3, r0
    lbz r0, 0x402(r3)
    and r3, r0, r31
    neg r0, r3
    or r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
@L_801D1C08:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
