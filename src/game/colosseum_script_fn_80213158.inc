    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x45
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r30, r3
    li r3, 0x0
    bl fn_801F54A4
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    mr r7, r31
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x0
    li r3, 0x1
    stb r0, lbl_8047B614(r13)
    bl fn_801EF8F4
    mr r3, r31
    li r4, 0x2e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802131E8
    mr r3, r31
    li r4, 0x2e
    bl fn_80202810
@L_802131E8:
    mr r3, r31
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021320C
    mr r3, r31
    li r4, 0x15
    bl fn_80202810
@L_8021320C:
    mr r3, r31
    li r4, 0x28
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213230
    mr r3, r31
    li r4, 0x28
    bl fn_80202810
@L_80213230:
    lis r4, lbl_8037889D@ha
    mr r3, r30
    addi r4, r4, lbl_8037889D@l
    li r5, 0x1
    bl fn_80211B94
    bl fn_801DA7AC
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
