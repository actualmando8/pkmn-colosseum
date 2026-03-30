    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    lbz r3, 0x2(r29)
    cmplwi r3, 0x0
    beq @L_800899C0
    bl fn_801EEAD0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_800899C0
    li r0, 0x1
    b @L_800899C4
@L_800899C0:
    li r0, 0x0
@L_800899C4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_800899E4
    mr r3, r28
    li r4, 0x0
    bl fn_801FC794
    li r3, 0x0
    b @L_80089B6C
@L_800899E4:
    lhz r4, 0x0(r29)
    mr r3, r28
    bl fn_801FC794
    mr r3, r28
    li r4, 0x0
    bl fn_801FC828
    lbz r4, 0x2(r29)
    mr r3, r28
    bl fn_801FC7B4
    lbz r3, 0x2(r29)
    cmplwi r3, 0x0
    beq @L_80089A1C
    lbz r4, 0x28(r29)
    bl fn_801EEE6C
@L_80089A1C:
    lbz r4, 0x3(r29)
    mr r3, r28
    bl fn_801FC7D4
    mr r31, r29
    li r30, 0x0
@L_80089A30:
    lhz r5, 0x4(r31)
    mr r3, r28
    rlwinm r4, r30, 0, 24, 31
    bl fn_801FC744
    addi r31, r31, 0x2
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_80089A30
    lhz r4, 0xc(r29)
    mr r3, r28
    bl fn_801FC784
    lbz r4, 0xe(r29)
    mr r3, r28
    bl fn_801FC7A4
    lbz r5, 0xf(r29)
    mr r3, r28
    li r4, 0x0
    bl fn_801FC808
    lbz r5, 0x10(r29)
    mr r3, r28
    li r4, 0x1
    bl fn_801FC808
    lbz r5, 0x11(r29)
    mr r3, r28
    li r4, 0x2
    bl fn_801FC808
    lbz r5, 0x12(r29)
    mr r3, r28
    li r4, 0x3
    bl fn_801FC808
    lbz r5, 0x13(r29)
    mr r3, r28
    li r4, 0x4
    bl fn_801FC808
    lbz r5, 0x14(r29)
    mr r3, r28
    li r4, 0x5
    bl fn_801FC808
    lha r5, 0x16(r29)
    mr r3, r28
    li r4, 0x0
    bl fn_801FC7E4
    lha r5, 0x18(r29)
    mr r3, r28
    li r4, 0x1
    bl fn_801FC7E4
    lha r5, 0x1a(r29)
    mr r3, r28
    li r4, 0x2
    bl fn_801FC7E4
    lha r5, 0x1c(r29)
    mr r3, r28
    li r4, 0x3
    bl fn_801FC7E4
    lha r5, 0x1e(r29)
    mr r3, r28
    li r4, 0x4
    bl fn_801FC7E4
    lha r5, 0x20(r29)
    mr r3, r28
    li r4, 0x5
    bl fn_801FC7E4
    lha r4, 0x22(r29)
    mr r3, r28
    bl fn_801FC6F4
    lbz r0, 0x24(r29)
    mr r3, r28
    extsb r4, r0
    bl fn_801FC6E4
    lbz r4, 0x25(r29)
    mr r3, r28
    bl fn_801FC6D4
    lbz r4, 0x26(r29)
    mr r3, r28
    bl fn_801FC694
    lbz r4, 0x27(r29)
    mr r3, r28
    bl fn_801FC684
    li r3, 0x1
@L_80089B6C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
