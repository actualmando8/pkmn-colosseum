    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_800D2B6C
    addi r3, r2, -27844
    li r4, 0xab
    addi r5, r2, -27836
    bl fn_80196E10
@L_800D2B6C:
    lfs f1, -27920(r2)
    lfs f0, 0xc(r31)
    fadds f0, f1, f0
    stfs f0, -23976(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
