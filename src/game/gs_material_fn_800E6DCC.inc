    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stfd f29, 0x30(r1)
    psq_st f29, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    lwz r0, 0x0(r3)
    mr r29, r3
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E725C
    lwz r30, 0x8(r29)
    cmplwi r30, 0x0
    bne @L_800E6E20
    li r31, 0x0
    b @L_800E6E24
    @L_800E6E20:
    lwz r31, 0x10(r30)
    @L_800E6E24:
    cmplwi r30, 0x0
    bne @L_800E6E3C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3e4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6E3C:
    addic. r0, r1, 0x14
    bne @L_800E6E58
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3e5
    bl fn_80196E10
    @L_800E6E58:
    lwz r4, 0x38(r30)
    cmplwi r30, 0x0
    lwz r3, 0x3c(r30)
    lwz r0, 0x40(r30)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bne @L_800E6E88
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2ec
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6E88:
    cmplwi r30, 0x0
    lfs f31, 0x1c(r30)
    bne @L_800E6EA4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2fa
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6EA4:
    cmplwi r30, 0x0
    lfs f30, 0x20(r30)
    bne @L_800E6EC0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x308
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6EC0:
    cmplwi r30, 0x0
    lfs f29, 0x24(r30)
    bne @L_800E6EDC
    la r3, lbl_8047CB9C(r2)
    li r4, 0x351
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6EDC:
    addic. r0, r1, 0x8
    bne @L_800E6EF4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x352
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E6EF4:
    lwz r4, 0x2c(r30)
    cmplwi r31, 0x0
    lwz r3, 0x30(r30)
    lwz r0, 0x34(r30)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    bne @L_800E6F24
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6F24:
    addic. r0, r1, 0x14
    bne @L_800E6F40
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E6F40:
    lwz r0, 0x14(r1)
    lwz r3, 0x18(r1)
    stw r0, 0x38(r31)
    lwz r0, 0x1c(r1)
    stw r3, 0x3c(r31)
    stw r0, 0x40(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E6FAC
    cmplwi r31, 0x0
    beq @L_800E6FAC
    bne @L_800E6F80
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6F80:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E6F9C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E6F9C
    li r3, 0x1
    @L_800E6F9C:
    cmpwi r3, 0x0
    bne @L_800E6FAC
    mr r3, r31
    bl fn_8019D620
    @L_800E6FAC:
    cmplwi r31, 0x0
    bne @L_800E6FC4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6FC4:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E6FE4
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E6FE4:
    stfs f31, 0x1c(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E703C
    cmplwi r31, 0x0
    beq @L_800E703C
    bne @L_800E7010
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7010:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E702C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E702C
    li r3, 0x1
    @L_800E702C:
    cmpwi r3, 0x0
    bne @L_800E703C
    mr r3, r31
    bl fn_8019D620
    @L_800E703C:
    cmplwi r31, 0x0
    bne @L_800E7054
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7054:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7074
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E7074:
    stfs f30, 0x20(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E70CC
    cmplwi r31, 0x0
    beq @L_800E70CC
    bne @L_800E70A0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E70A0:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E70BC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E70BC
    li r3, 0x1
    @L_800E70BC:
    cmpwi r3, 0x0
    bne @L_800E70CC
    mr r3, r31
    bl fn_8019D620
    @L_800E70CC:
    cmplwi r31, 0x0
    bne @L_800E70E4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E70E4:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7104
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E7104:
    stfs f29, 0x24(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E715C
    cmplwi r31, 0x0
    beq @L_800E715C
    bne @L_800E7130
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7130:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E714C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E714C
    li r3, 0x1
    @L_800E714C:
    cmpwi r3, 0x0
    bne @L_800E715C
    mr r3, r31
    bl fn_8019D620
    @L_800E715C:
    cmplwi r31, 0x0
    bne @L_800E7174
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7174:
    addic. r0, r1, 0x8
    bne @L_800E718C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E718C:
    lwz r0, 0x8(r1)
    lwz r3, 0xc(r1)
    stw r0, 0x2c(r31)
    lwz r0, 0x10(r1)
    stw r3, 0x30(r31)
    stw r0, 0x34(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E71F8
    cmplwi r31, 0x0
    beq @L_800E71F8
    bne @L_800E71CC
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E71CC:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E71E8
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E71E8
    li r3, 0x1
    @L_800E71E8:
    cmpwi r3, 0x0
    bne @L_800E71F8
    mr r3, r31
    bl fn_8019D620
    @L_800E71F8:
    mr r3, r30
    bl fn_801A02B0
    cmplwi r31, 0x0
    mr r30, r3
    beq @L_800E724C
    bne @L_800E7220
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7220:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E723C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E723C
    li r3, 0x1
    @L_800E723C:
    cmpwi r3, 0x0
    beq @L_800E724C
    mr r3, r31
    bl fn_8019D9DC
    @L_800E724C:
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 15, 13
    stw r0, 0x0(r29)
    stw r30, 0x8(r29)
    @L_800E725C:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    psq_l f29, 0x38(r1), 0, 0
    lfd f29, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0x64(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
