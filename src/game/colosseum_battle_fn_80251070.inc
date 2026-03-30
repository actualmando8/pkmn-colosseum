    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r6
    bl fn_802376EC
    clrlwi r31, r3, 16
    mr r3, r29
    mr r4, r30
    bl fn_802376EC
    clrlwi r0, r3, 16
    xoris r4, r0, 0x8000
    subf r3, r0, r31
    subf r0, r31, r0
    addc r3, r3, r4
    subfe r3, r3, r3
    andc r3, r0, r3
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
