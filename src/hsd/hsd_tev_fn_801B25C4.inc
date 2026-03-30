    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    stw r30, 0x8(r1)
    mr r30, r3
    b @L_801B2620
@L_801B25E4:
    lis r3, -32713
    rlwinm r4, r31, 3, 0, 28
    addi r0, r3, -12376
    add r3, r0, r4
    lwz r0, 0x0(r3)
    and. r0, r30, r0
    beq @L_801B261C
    lis r3, -32713
    rlwinm r0, r31, 3, 0, 28
    addi r3, r3, -12376
    add r3, r3, r0
    lwz r12, 0x4(r3)
    mtctr r12
    bctrl
@L_801B261C:
    addi r31, r31, 0x1
@L_801B2620:
    lis r3, -32713
    rlwinm r4, r31, 3, 0, 28
    addi r0, r3, -12376
    add r3, r0, r4
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801B25E4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
