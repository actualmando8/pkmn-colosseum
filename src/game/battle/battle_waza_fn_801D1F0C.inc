    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    cmpwi r30, 0x0
    mr r31, r3
    blt @L_801D1F54
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    lhz r0, 0x400(r3)
    cmpw r30, r0
    blt @L_801D1F5C
@L_801D1F54:
    li r3, -0x1
    b @L_801D1F64
@L_801D1F5C:
    rlwinm r0, r30, 1, 0, 30
    lhzx r3, r31, r0
@L_801D1F64:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
