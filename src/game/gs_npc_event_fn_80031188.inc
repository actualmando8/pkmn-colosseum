    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r5, 0x64(r4)
    lha r0, 0x6(r4)
    andis. r4, r5, 0xa140
    lbz r3, 0x8b(r3)
    cmpwi r0, 0xf6b
    or r31, r4, r3
    beq @L_800311C4
    bge @L_80031214
    cmpwi r0, 0xf6a
    bge @L_800311DC
    b @L_80031214
    @L_800311C4:
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    li r6, 0x4412
    bl fn_800FB680
    b @L_80031214
    @L_800311DC:
    lis r3, lbl_803A2688@ha
    li r4, 0x1
    addi r3, r3, lbl_803A2688@l
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0x4d
    mr r4, r0
    bl fn_80132A38
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    li r6, 0x4413
    bl fn_800FB680
    @L_80031214:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
