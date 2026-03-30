    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr. r30, r3
    stw r29, 0x14(r1)
    beq @L_801C27D4
    lis r3, lbl_80466DB8@ha
    addi r3, r3, lbl_80466DB8@l
    bl fn_801AA4CC
    mr. r29, r3
    bne @L_801C26B4
    la r3, lbl_8047DF40(r2)
    li r4, 0x1e9
    la r5, lbl_8047DF4C(r2)
    bl fn_80196E10
    @L_801C26B4:
    mr r3, r29
    li r4, 0x0
    li r5, 0x1c
    bl memset
    lis r0, 0x4000
    lfs f0, lbl_8047DF50(r2)
    stw r0, 0x0(r29)
    mr r0, r29
    mr. r31, r0
    stfs f0, 0x10(r29)
    lwz r3, 0x0(r30)
    beq @L_801C26F4
    lwz r0, 0x0(r31)
    rlwinm r3, r3, 0, 2, 3
    or r0, r0, r3
    stw r0, 0x0(r31)
    @L_801C26F4:
    cmplwi r31, 0x0
    beq @L_801C2704
    lfs f0, lbl_8047DF54(r2)
    stfs f0, 0x8(r31)
    @L_801C2704:
    cmplwi r31, 0x0
    lfs f0, 0x4(r30)
    beq @L_801C2714
    stfs f0, 0xc(r31)
    @L_801C2714:
    lwz r3, 0x8(r30)
    bl fn_80199794
    cmplwi r31, 0x0
    mr r29, r3
    beq @L_801C2740
    lwz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801C273C
    lwz r3, 0x14(r31)
    bl fn_8019B5E8
    @L_801C273C:
    stw r29, 0x14(r31)
    @L_801C2740:
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    beq @L_801C27CC
    lwz r4, 0xc(r30)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    mr. r29, r3
    beq @L_801C27A0
    cmplwi r29, 0x0
    beq @L_801C27AC
    lhz r3, 0x4(r29)
    addi r0, r3, 0x1
    sth r0, 0x4(r29)
    lhz r0, 0x4(r29)
    cmplwi r0, 0xffff
    bne @L_801C27AC
    lis r3, lbl_802757BC@ha
    lis r5, lbl_802757C8@ha
    addi r3, r3, lbl_802757BC@l
    li r4, 0x5d
    addi r5, r5, lbl_802757C8@l
    bl fn_80196E10
    b @L_801C27AC
    @L_801C27A0:
    lwz r3, 0xc(r30)
    bl fn_801A0FBC
    mr r29, r3
    @L_801C27AC:
    cmplwi r31, 0x0
    beq @L_801C27CC
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801C27C8
    lwz r3, 0x18(r31)
    bl fn_801A05EC
    @L_801C27C8:
    stw r29, 0x18(r31)
    @L_801C27CC:
    mr r3, r31
    b @L_801C27D8
    @L_801C27D4:
    li r3, 0x0
    @L_801C27D8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
