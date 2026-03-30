    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801022F8
    lbz r4, 0x95(r3)
    lbz r0, 0x94(r3)
    extsb r3, r4
    extsb r0, r0
    add r31, r3, r0
    b @L_801022FC
@L_801022F8:
    li r31, -0x1
@L_801022FC:
    cmpwi r31, -0x1
    bne @L_8010230C
    li r3, 0x0
    b @L_8010237C
@L_8010230C:
    mr r3, r29
    bl fn_8005DA18
    cmplwi r3, 0x0
    bne @L_80102324
    li r3, 0x0
    b @L_8010237C
@L_80102324:
    lha r0, 0x4(r3)
    li r29, 0x0
    mr r30, r0
@L_80102330:
    extsh r3, r30
    bl fn_8005D934
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_8010235C
    cmpw r29, r31
    bne @L_80102358
    extsh r3, r30
    b @L_8010237C
@L_80102358:
    addi r29, r29, 0x1
@L_8010235C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80102378
    lha r0, 0x18(r3)
    mr r30, r0
    b @L_80102330
@L_80102378:
    li r3, 0x0
@L_8010237C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
