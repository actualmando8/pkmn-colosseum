    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    li r0, 0x0
    addi r3, r13, -24972
    stw r0, -24972(r13)
    stw r0, 0x4(r3)
    stw r0, -24976(r13)
    bl fn_80113F48
    mr r0, r3
    li r3, 0xb5d
    mr r30, r0
    bl fn_801906A0
    mr r31, r3
    lis r4, -32721
    rlwinm r0, r31, 2, 0, 29
    mr r3, r30
    addi r4, r4, -5008
    lwzx r12, r4, r0
    mtctr r12
    bctrl
    addi r4, r31, 0x1
    cmplwi r4, 0x1f
    blt @L_8008C76C
    li r4, 0x0
@L_8008C76C:
    li r3, 0xb5d
    bl fn_8019075C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
