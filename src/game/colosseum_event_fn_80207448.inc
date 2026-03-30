    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x113
    stw r0, 0x14(r1)
    li r6, 0x0
    li r7, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x114
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x115
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x116
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x117
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x118
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11d
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11f
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
