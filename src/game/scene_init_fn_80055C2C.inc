    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lbz r31, 0x95(r3)
    extsb r31, r31
    cmpwi r31, 0x0
    blt @L_80055C68
    bl fn_801347E0
    extsb r0, r3
    cmpw r31, r0
    blt @L_80055C70
    @L_80055C68:
    li r3, 0x0
    b @L_80055CB8
    @L_80055C70:
    bl fn_801347D8
    extsb r30, r3
    mr r4, r31
    li r3, 0x0
    bl fn_801348EC
    extsb r4, r3
    cmpwi r4, 0x0
    bge @L_80055C98
    li r3, 0x0
    b @L_80055CB8
    @L_80055C98:
    li r3, 0x34
    bl fn_80132A38
    mr r4, r30
    li r3, 0x35
    bl fn_80132A38
    li r0, 0x1b7f
    li r3, 0x0
    stw r0, 0x4c(r29)
    @L_80055CB8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
