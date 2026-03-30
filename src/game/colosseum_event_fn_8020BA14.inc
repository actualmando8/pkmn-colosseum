    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F54A4
    mr r3, r31
    bl fn_8020D908
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    bl fn_80212D6C
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
