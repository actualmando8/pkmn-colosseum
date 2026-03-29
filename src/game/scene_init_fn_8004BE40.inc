    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_8004BE68
    bl fn_801D1F7C
    cmpw r31, r3
    blt @L_8004BE70
    @L_8004BE68:
    li r3, -0x1
    b @L_8004BE7C
    @L_8004BE70:
    lwz r3, lbl_8047A500(r13)
    slwi r0, r31, 1
    lhzx r3, r3, r0
    @L_8004BE7C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
