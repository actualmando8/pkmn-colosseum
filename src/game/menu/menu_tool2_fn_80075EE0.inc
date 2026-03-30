    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -25096(r13)
    cmpwi r0, 0x0
    bne @L_80075F20
    bl fn_800FF560
    lis r5, -32761
    mr r4, r3
    addi r8, r5, 24396
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    b @L_80075F30
@L_80075F20:
    li r3, 0x46a
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
@L_80075F30:
    lfs f1, -30168(r2)
    li r3, 0x2
    bl fn_801C41C8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
