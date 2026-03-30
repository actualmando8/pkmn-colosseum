    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0xd4(r1)
    stmw r28, 0xc0(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r31, r0
    mr r4, r31
    bl fn_801F025C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80207BF4
    mr r29, r3
    li r3, 0x0
    bl fn_801F3178
    li r3, 0x0
    bl fn_801F3074
    mr r3, r31
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_801F76B8
    cmpwi r3, 0x0
    bne @L_8021F138
    mr r3, r30
    li r4, 0x4a
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F138
    mr r3, r31
    li r4, 0x2
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021F138
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x1a
    beq @L_8021F138
    mr r3, r30
    li r4, 0x0
    li r5, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_801F75F8
    mr r3, r30
    li r4, 0x4a
    bl fn_801F6D9C
    extsh r0, r3
    mr r3, r31
    subfic r0, r0, 0x5
    clrlslwi r4, r0, 17, 1
    bl fn_80203B5C
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    addi r3, r1, 0x8
    mr r28, r0
    mr r4, r28
    bl fn_8020A2B8
    mr r3, r28
    mr r7, r29
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r28
    bl fn_80209FAC
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x1(r3)
    cmplwi r0, 0x12
    bne @L_8021F0C4
    lis r3, lbl_80378DAF@ha
    addi r0, r3, lbl_80378DAF@l
    mr r4, r0
    b @L_8021F0F4
@L_8021F0C4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x11
    beq @L_8021F0D8
    cmplwi r0, 0x14
    bne @L_8021F0E8
@L_8021F0D8:
    lis r3, lbl_80378D7C@ha
    addi r0, r3, lbl_80378D7C@l
    mr r4, r0
    b @L_8021F0F4
@L_8021F0E8:
    lis r3, lbl_80378DE2@ha
    addi r0, r3, lbl_80378DE2@l
    mr r4, r0
@L_8021F0F4:
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8020A2B8
    mr r3, r30
    li r4, 0x0
    li r5, 0x8
    li r6, 0x0
    bl fn_801F76B8
    cmpwi r3, 0x0
    bne @L_8021F138
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    b @L_8021F1B8
@L_8021F138:
    mr r3, r31
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x36
    bne @L_8021F164
    mr r3, r31
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_8021F164:
    mr r3, r31
    li r4, 0x0
    bl fn_8022E6F0
    mr r3, r31
    bl fn_8022E410
    mr r3, r31
    bl fn_8022D084
    mr r3, r31
    li r4, 0x1
    bl fn_8022E6F0
    li r3, 0x0
    bl fn_801F2F3C
    mr r3, r30
    li r4, 0x0
    li r5, 0x8
    li r6, 0x0
    li r7, 0x0
    bl fn_801F75F8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
@L_8021F1B8:
    lmw r28, 0xc0(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
