    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x102
    stw r0, 0x14(r1)
    li r6, 0x0
    li r7, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x103
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x105
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x106
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x107
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x108
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x109
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10a
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10b
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10c
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10d
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10e
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10f
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x110
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x111
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
