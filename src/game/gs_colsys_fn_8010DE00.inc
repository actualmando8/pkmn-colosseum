    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stw r31, 0x7c(r1)
    mr r31, r5
    stw r30, 0x78(r1)
    mr r30, r4
    addi r4, r1, 0x8
    stw r29, 0x74(r1)
    mr r29, r3
    bl fn_8010E138
    cmpwi r3, 0x0
    bgt @L_8010DE3C
    li r3, 0x0
    b @L_8010DED4
@L_8010DE3C:
    lfs f2, 0x4(r29)
    lfs f1, 0x8(r1)
    lfs f0, -26560(r2)
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8010DE58
    b @L_8010DE5C
@L_8010DE58:
    fneg f1, f1
@L_8010DE5C:
    subi r0, r3, 0x1
    fmr f3, f1
    addi r4, r1, 0x14
    lfs f0, -26560(r2)
    li r6, 0x0
    li r5, 0x1
    mtctr r0
    cmpwi r3, 0x1
    ble @L_8010DEB4
@L_8010DE80:
    lfs f1, 0x0(r4)
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8010DE94
    b @L_8010DE98
@L_8010DE94:
    fneg f1, f1
@L_8010DE98:
    fcmpo cr0, f3, f1
    ble @L_8010DEA8
    mr r6, r5
    fmr f3, f1
@L_8010DEA8:
    addi r4, r4, 0xc
    addi r5, r5, 0x1
    bdnz @L_8010DE80
@L_8010DEB4:
    mulli r6, r6, 0xc
    addi r5, r1, 0x10
    addi r4, r1, 0x11
    li r3, 0x1
    lbzx r0, r5, r6
    stb r0, 0x0(r30)
    lbzx r0, r4, r6
    stb r0, 0x0(r31)
@L_8010DED4:
    lwz r0, 0x84(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    lwz r29, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
