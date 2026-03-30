    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32728
    stw r0, 0x34(r1)
    addi r6, r4, -25504
    stmw r29, 0x24(r1)
    mr r29, r3
    addi r31, r1, 0x8
    li r30, 0x0
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    lhz r0, 0xc(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    sth r0, 0x14(r1)
    b @L_80207428
@L_80207408:
    clrlslwi r0, r30, 24, 1
    mr r3, r29
    lhzx r5, r31, r0
    li r4, 0x0
    li r6, 0x0
    li r7, 0x6
    bl fn_801254B4
    addi r30, r30, 0x1
@L_80207428:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x7
    blt @L_80207408
    lmw r29, 0x24(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
