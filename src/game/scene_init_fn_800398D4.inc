    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lha r3, 0x50(r4)
    li r5, 0x150
    lha r0, 0x52(r4)
    li r6, 0x10d
    subfic r3, r3, 0x118
    subfic r4, r0, 0x8b
    bl fn_800FE38C
    lwz r4, 0x88(r31)
    addi r3, r31, 0x94
    bl fn_80039644
    bl fn_800FE35C
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
