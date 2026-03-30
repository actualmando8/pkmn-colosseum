    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010A8B4
    li r3, 0x0
    b @L_8010AAE8
@L_8010A8B4:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_8010A8D0
    li r3, 0x0
    b @L_8010AAE8
    b @L_8010A8D0
@L_8010A8CC:
    bl fn_800F0308
@L_8010A8D0:
    bl fn_8010B560
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010A8CC
    lbz r0, lbl_8047AD44(r13)
    cmplwi r0, 0x0
    bne @L_8010A8FC
    li r0, 0x1
    li r3, 0x4
    stb r0, lbl_8047AD44(r13)
    bl fn_801DADC0
@L_8010A8FC:
    cmplwi r31, 0x0
    beq @L_8010A954
    li r0, 0x0
    stb r0, 0x1(r31)
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_8010A934
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_8010A94C
    bl fn_801DB100
    li r0, 0x0
    stw r0, 0x24(r31)
    b @L_8010A94C
@L_8010A934:
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_8010A94C
    bl fn_800E4BF4
    li r0, 0x0
    stw r0, 0x24(r31)
@L_8010A94C:
    li r0, 0x0
    stb r0, 0x0(r31)
@L_8010A954:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x0(r31)
    stb r0, 0x1(r31)
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r31)
    stw r3, 0x14(r31)
    stw r0, 0x18(r31)
    lwz r3, 0xc(r31)
    lwz r0, 0x10(r31)
    stw r3, 0x1c(r31)
    stw r0, 0x20(r31)
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_8010AA34
    lhz r3, 0x18(r31)
    lwz r4, 0x1c(r31)
    lbz r5, 0x22(r31)
    bl fn_801DE190
    stw r3, 0x24(r31)
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    bne @L_8010A9C0
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x0(r31)
    b @L_8010AAE8
@L_8010A9C0:
    bl fn_801DAC3C
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_8010A9E0
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x0(r31)
    b @L_8010AAE8
@L_8010A9E0:
    li r4, 0x0
    bl fn_800E4014
    lhz r4, 0x1a(r31)
    cmplwi r4, 0x0
    beq @L_8010AA94
    lwz r3, 0x24(r31)
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010AA94
    lwz r3, 0x24(r31)
    li r5, 0x4
    lhz r4, 0x1a(r31)
    bl fn_801DA914
    lwz r3, 0x24(r31)
    li r5, 0x4
    lhz r4, 0x1a(r31)
    bl fn_801DA9E8
    b @L_8010AA94
@L_8010AA34:
    lwz r3, 0x18(r31)
    bl fn_80113D58
    stw r3, 0x24(r31)
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    bne @L_8010AA54
    li r3, 0x0
    b @L_8010AAE8
@L_8010AA54:
    bl fn_800EC1BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010AA84
    lwz r3, 0x24(r31)
    lwz r4, 0x1c(r31)
    bl fn_800ECCA8
    lwz r3, 0x24(r31)
    lfs f1, lbl_8047CE70(r13)
    bl fn_800EC9DC
    lwz r3, 0x24(r31)
    bl fn_800EC990
@L_8010AA84:
    lwz r3, 0x24(r31)
    li r4, 0x0
    bl fn_800E4014
    lwz r30, 0x24(r31)
@L_8010AA94:
    mr r3, r30
    li r4, 0x1
    bl fn_800ECB74
    lfs f1, lbl_8047CE78(r13)
    addi r3, r1, 0x8
    lfs f2, lbl_8047CE7C(r13)
    lfs f3, lbl_8047CE80(r13)
    bl fn_800E01F4
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_800E4170
    mr r3, r31
    bl fn_8010AB00
    li r0, 0x2
    stb r0, 0x0(r31)
    lbz r0, 0x1(r31)
    cmplwi r0, 0x1
    beq @L_8010A8FC
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x28(r31)
@L_8010AAE8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

