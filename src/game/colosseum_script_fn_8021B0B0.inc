    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x25
    li r6, 0x0
    stmw r30, 0x8(r1)
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021B184
    li r3, 0x0
    bl fn_801F2A7C
    mr. r30, r3
    beq @L_8021B184
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801FB1C0
    mr. r31, r3
    beq @L_8021B184
    mr r3, r30
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    mr r3, r30
    mullw r31, r31, r0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr. r30, r3
    beq @L_8021B184
    mr r4, r31
    bl fn_801294C4
    mr r4, r31
    li r3, 0x2f
    bl fn_80132A38
    mr r3, r30
    li r4, 0x1
    li r5, 0x0
    bl fn_8012A5B0
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    lis r4, lbl_8037939C@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037939C@l
    li r5, 0x0
    bl fn_80211B94
@L_8021B184:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
