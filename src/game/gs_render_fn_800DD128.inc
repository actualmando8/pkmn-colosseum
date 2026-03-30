    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_800DD150
    la r3, lbl_8047CA90(r2)
    li r4, 0xab
    la r5, lbl_8047CA98(r2)
    bl fn_80196E10
    @L_800DD150:
    lfs f1, lbl_8047CA70(r2)
    lfs f0, 0xc(r31)
    fadds f0, f1, f0
    stfs f0, lbl_8047AAF4(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
