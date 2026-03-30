    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r30, 0x8(r1)
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x14
    li r4, 0x0
    bl fn_801F025C
    mr r31, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FAB4
    mr r3, r31
    li r4, 0x14
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FA50
    mr r3, r31
    bl fn_801FEF74
    cmpwi r3, 0x0
    ble @L_8021FA50
    mr r3, r31
    li r4, 0x14
    bl fn_80202810
    mr r3, r31
    bl fn_802086B0
    mr r4, r31
    mr r5, r31
    li r3, 0xa4
    li r6, 0x0
    li r7, 0x0
    bl fn_8020F108
@L_8021FA34:
    li r3, 0x6
    bl fn_801DA5C4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021FA50
    bl fn_800F0308
    b @L_8021FA34
@L_8021FA50:
    mr r3, r31
    bl fn_802086B0
    mr r3, r31
    li r4, 0x0
    bl fn_80208ED0
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
    mr r3, r31
    li r4, 0x1
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x2
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x3
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x4
    bl fn_80208ED0
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_8026532C
@L_8021FAB4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
