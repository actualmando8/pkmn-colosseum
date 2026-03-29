    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    li r4, 0x1
    bl fn_800141BC
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_80022E94
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_80014118
    @L_80022E94:
    mr r3, r31
    bl fn_80014198
    cmpwi r31, 0x0
    blt @L_80022EC4
    lis r4, 0x596
    mr r3, r29
    addi r4, r4, 0x10
    bl fn_800190D8
    li r0, 0x1
    li r3, 0x2
    stw r0, 0x0(r30)
    b @L_80022EC8
    @L_80022EC4:
    li r3, 0x1
    @L_80022EC8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
