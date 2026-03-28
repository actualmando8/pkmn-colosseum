    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    lis r8, lbl_803A9A60@ha
    addi r8, r8, lbl_803A9A60@l
    lwz r0, 0x4(r8)
    cmpwi r0, 0x1
    beq @L_80061A88
    bge @L_80061B60
    cmpwi r0, 0x0
    bge @L_80061A74
    b @L_80061B60
@L_80061A74:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_80061B60
@L_80061A88:
    bl fn_80061D34
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80061B50
    mulli r4, r29, 0xb4
    lis r3, lbl_803A9A60@ha
    lha r6, 0x6(r28)
    lis r5, lbl_802EF0A8@ha
    addi r0, r3, lbl_803A9A60@l
    add r3, r0, r4
    slwi r0, r30, 2
    addi r4, r5, lbl_802EF0A8@l
    add r3, r3, r0
    lfs f0, 0x94(r3)
    mulli r0, r6, 0x1c
    fctiwz f0, f0
    add r3, r4, r0
    lha r3, 0x2(r3)
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lbz r0, 0x4(r28)
    mr r5, r27
    li r3, 0x0
    li r4, 0x0
    rlwinm r0, r0, 0, 31, 29
    li r6, 0x314
    extsb r0, r0
    li r7, 0x0
    stb r0, 0x4(r28)
    bl fn_801040F0
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_80061240
    b @L_80061B60
@L_80061B50:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
@L_80061B60:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
