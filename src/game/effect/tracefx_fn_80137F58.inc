    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    lwz r0, 0x14(r3)
    lhz r3, 0x1c(r3)
    cmplwi r0, 0x0
    lhz r31, 0x1e(r27)
    bne @L_80137F88
    li r3, 0x0
    b @L_801380C0
@L_80137F88:
    cmplwi r3, 0x1
    ble @L_801380BC
    cmplwi r31, 0x1
    ble @L_801380BC
    bl fn_800D2248
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lwz r3, 0x10(r27)
    bl fn_800D7820
    lwz r4, 0x14(r27)
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D6A00
    clrlslwi r3, r31, 17, 1
    bl fn_800D67BC
    lhz r0, 0x22(r27)
    li r28, 0x0
    lwz r3, 0x8(r27)
    subf r0, r31, r0
    lwz r30, 0x0(r27)
    rlwinm r0, r0, 4, 0, 27
    add r29, r3, r0
    b @L_801380A4
@L_80138028:
    lfs f1, 0x0(r30)
    lfs f2, 0x4(r30)
    lfs f3, 0x8(r30)
    bl fn_800D6680
    lbz r4, 0x18(r27)
    li r3, 0x0
    lbz r5, 0x19(r27)
    lbz r6, 0x1a(r27)
    lbz r7, 0x1b(r27)
    bl fn_800D5CB8
    lfs f1, 0x0(r29)
    li r3, 0x0
    lfs f2, 0x4(r29)
    bl fn_800D59B8
    lfs f1, 0xc(r30)
    lfs f2, 0x10(r30)
    lfs f3, 0x14(r30)
    bl fn_800D6680
    lbz r4, 0x18(r27)
    li r3, 0x0
    lbz r5, 0x19(r27)
    lbz r6, 0x1a(r27)
    lbz r7, 0x1b(r27)
    bl fn_800D5CB8
    lfs f1, 0x8(r29)
    li r3, 0x0
    lfs f2, 0xc(r29)
    bl fn_800D59B8
    lwz r30, 0x1c(r30)
    addi r29, r29, 0x10
    addi r28, r28, 0x1
@L_801380A4:
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r31
    blt @L_80138028
    bl fn_800D6728
    li r3, 0x1
    b @L_801380C0
@L_801380BC:
    li r3, 0x0
@L_801380C0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
