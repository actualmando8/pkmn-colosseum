    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xd6
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    mr r3, r4
    li r4, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r31, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r31
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x99
    li r6, 0x0
    bl fn_8012640C
    lwz r0, 0x14(r1)
    rlwinm r3, r3, 0, 16, 31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
