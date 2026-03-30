    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_80315598@ha
    mr r8, r3
    stw r0, 0x24(r1)
    addi r0, r4, lbl_80315598@l
    stw r7, 0x8(r1)
    addi r7, r1, 0x8
    stw r6, 0x10(r1)
    mr r6, r0
    stw r5, 0xc(r1)
    lhz r0, 0xe(r3)
    lwz r3, 0x8(r3)
    lwz r4, 0x10(r8)
    slwi r5, r0, 5
    bl fn_800DA578
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
