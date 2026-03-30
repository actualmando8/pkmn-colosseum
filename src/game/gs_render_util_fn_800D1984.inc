    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x3(r3)
    cmplwi r0, 0x0
    beq @L_800D1A20
    lwz r3, 0xc(r30)
    bl fn_80196BB8
    lwz r0, 0x110(r30)
    cmplw r31, r0
    bgt @L_800D1A20
    stw r31, 0x114(r30)
    lwz r3, 0x8(r30)
    lwz r0, 0x114(r30)
    lwz r4, 0x4(r3)
    rlwinm r0, r0, 2, 0, 29
    lwz r3, 0xc(r30)
    lwzx r4, r4, r0
    bl fn_80196B10
    lwz r3, 0xc(r30)
    lfs f1, -27912(r2)
    bl fn_80196698
    lfs f0, -27912(r2)
    lis r3, -32755
    addi r6, r3, 11076
    li r4, 0x2
    stfs f0, -23976(r13)
    lis r3, 0x1
    subi r5, r3, 0x1
    li r7, 0x0
    lwz r3, 0xc(r30)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, -23976(r13)
    stfs f0, 0x120(r30)
@L_800D1A20:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
