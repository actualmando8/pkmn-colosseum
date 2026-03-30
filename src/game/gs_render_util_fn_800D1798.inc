    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x3(r3)
    cmplwi r0, 0x0
    beq @L_800D1844
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x4(r31)
    stb r0, 0x124(r31)
    stb r3, 0x125(r31)
    lbz r0, 0x3(r31)
    lfs f0, 0x11c(r31)
    cmplwi r0, 0x0
    beq @L_800D1844
    stfs f0, 0x11c(r31)
    lis r3, -32740
    addi r6, r3, 636
    lis r4, 0x1
    lwz r3, 0xc(r31)
    subi r5, r4, 0x1
    lfd f1, -27880(r2)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
    lwz r3, 0xc(r31)
    lfs f1, 0x11c(r31)
    bl fn_80196698
    lwz r3, 0xc(r31)
    bl fn_801966FC
    lis r3, -32740
    lis r4, 0x1
    addi r6, r3, 636
    lwz r3, 0xc(r31)
    subi r5, r4, 0x1
    lfs f1, 0x118(r31)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
@L_800D1844:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
