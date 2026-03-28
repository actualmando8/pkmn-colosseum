    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lha r0, 0x6(r4)
    fctiwz f0, f1
    lis r5, lbl_802EF0A8@ha
    mulli r6, r0, 0x1c
    stfd f0, 0x8(r1)
    addi r0, r5, lbl_802EF0A8@l
    add r5, r0, r6
    lwz r0, 0xc(r1)
    lha r5, 0x2(r5)
    add r0, r5, r0
    extsh r0, r0
    sth r0, 0x50(r4)
    lha r7, 0x84(r3)
    lha r6, 0x50(r4)
    lha r5, 0x86(r3)
    lha r0, 0x52(r4)
    add r3, r7, r6
    extsh r3, r3
    add r0, r5, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
