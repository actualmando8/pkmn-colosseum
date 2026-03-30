    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    bne @L_8025FF40
    li r3, 0x0
    li r4, 0xc
    bl fn_80129280
@L_8025FF40:
    li r5, 0x0
    b @L_8025FF74
@L_8025FF48:
    rlwinm r0, r5, 0, 16, 31
    mulli r4, r0, 0xc
    addi r4, r4, 0x4
    lhzx r0, r3, r4
    rlwinm. r0, r0, 0, 16, 16
    beq @L_8025FF64
    li r31, 0x1
@L_8025FF64:
    lhzx r0, r3, r4
    addi r5, r5, 0x1
    rlwinm r0, r0, 0, 18, 31
    sthx r0, r3, r4
@L_8025FF74:
    lhz r0, 0x0(r3)
    rlwinm r4, r5, 0, 16, 31
    cmplw r4, r0
    blt @L_8025FF48
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
