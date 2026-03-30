    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801F54A4
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_80205184
    mr r0, r3
    mr r3, r30
    mr r31, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r29
    mr r29, r0
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802270E4
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_80227090
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x90
    beq @L_80227090
    cmplwi r0, 0xa4
    beq @L_80227090
    lis r4, lbl_80378964@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378964@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802270B0
@L_80227090:
    lis r4, lbl_80379F58@ha
    mr r3, r31
    addi r5, r4, lbl_80379F58@l
    mr r4, r30
    addis r6, r5, 0x1
    mr r5, r29
    lbz r6, 0x60a1(r6)
    bl fn_8020955C
@L_802270B0:
    lis r4, lbl_80379F58@ha
    lwz r3, lbl_8047B610(r13)
    addi r4, r4, lbl_80379F58@l
    addis r5, r4, 0x1
    addi r0, r3, 0x1
    lbz r4, 0x6002(r5)
    lbz r3, 0x60a1(r5)
    addi r4, r4, 0x1
    stw r0, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stb r4, 0x6002(r5)
    stb r0, 0x60a1(r5)
    b @L_80227164
@L_802270E4:
    li r3, 0x0
    bl fn_80261E7C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80227158
    li r4, 0x57
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80227158
    mr r3, r30
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r29
    mr r4, r31
    li r5, 0x1
    bl fn_80265598
@L_80227158:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80227164:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
