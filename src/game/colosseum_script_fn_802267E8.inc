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
    lwz r5, lbl_8047B610(r13)
    rlwinm r30, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    bl fn_801F025C
    mr r4, r30
    mr r30, r3
    li r5, 0x1
    bl fn_802656AC
    mr r31, r3
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226850
    mr r3, r31
    li r4, 0x1
    bl fn_80011C78
@L_80226850:
    mr r3, r30
    bl fn_802038A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226870
    mr r3, r30
    li r4, 0x1
    bl fn_80207C24
@L_80226870:
    mr r3, r30
    li r4, 0x14
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802268F4
    mr r3, r30
    bl fn_801FEF74
    cmpwi r3, 0x0
    bgt @L_802268F4
@L_80226898:
    li r3, 0x6
    bl fn_801DA5C4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802268B4
    bl fn_800F0308
    b @L_80226898
@L_802268B4:
    mr r3, r30
    li r4, 0x14
    bl fn_80202810
    lis r4, lbl_80379A22@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379A22@l
    li r5, 0x0
    bl fn_80211B94
    mr r3, r30
    li r4, 0x14
    li r5, 0x0
    bl fn_8020248C
    mr r3, r30
    li r4, 0x14
    li r5, 0x0
    bl fn_80201764
@L_802268F4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
