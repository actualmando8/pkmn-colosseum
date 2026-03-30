    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, -22208(r2)
    lfs f3, -22188(r2)
    fmr f2, f1
    lfs f4, -22184(r2)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    li r7, 0x7
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
