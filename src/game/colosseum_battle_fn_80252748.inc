    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r6
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r4, r30
    bl fn_802376EC
    mr r31, r3
    mr r3, r27
    mr r4, r29
    mr r5, r28
    mr r6, r30
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_80211170
    clrlwi r4, r31, 16
    cmpw r4, r3
    bgt @L_802527A8
    subi r3, r4, 0x1
@L_802527A8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
