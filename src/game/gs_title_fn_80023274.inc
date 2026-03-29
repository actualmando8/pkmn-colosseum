    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    lfs f1, lbl_8047B8A4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    mr r3, r30
    mr r4, r31
    bl fn_80097B04
    mr r31, r3
    cmpwi r31, 0x4
    blt @L_800232C0
    li r31, -0x1
    @L_800232C0:
    lfs f1, lbl_8047B8A4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    extsb r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
