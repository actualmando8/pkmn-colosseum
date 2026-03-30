    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r29, r3
    li r4, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r30
    li r4, 0x0
    li r5, 0xec
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 24, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235878
    mr r3, r30
    li r4, 0x19
    bl fn_802026E4
    b @L_8023587C
@L_80235878:
    li r3, 0x0
@L_8023587C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235894
    cmplwi r31, 0x6
    ble @L_80235894
    li r31, 0x6
@L_80235894:
    mr r3, r31
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
