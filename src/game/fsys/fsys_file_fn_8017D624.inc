    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    stw r30, 0x8(r1)
    lwz r30, lbl_8047B1B8(r13)
    b @L_8017D660
    @L_8017D644:
    slwi r0, r31, 3
    lwzx r3, r30, r0
    bl fn_8017F800
    slwi r0, r31, 3
    li r3, -0x1
    stwx r3, r30, r0
    addi r31, r31, 0x1
    @L_8017D660:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r31, r0
    blt @L_8017D644
    li r0, 0x0
    stw r0, lbl_8047B1BC(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
