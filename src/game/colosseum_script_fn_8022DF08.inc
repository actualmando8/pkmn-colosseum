    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r4, 0x1
    bl fn_801F453C
    cmplwi r31, 0x0
    rlwinm r28, r3, 0, 24, 31
    beq @L_8022E1B0
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022E1B0
    mr r3, r31
    bl fn_80207BF4
    mr r30, r3
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r30, 0, 16, 31
    mr r30, r3
    cmpwi r0, 0x36
    beq @L_8022E17C
    bge @L_8022DFB4
    cmpwi r0, 0x2c
    beq @L_8022DFC0
    bge @L_8022E1B0
    cmpwi r0, 0x3
    beq @L_8022E0D4
    b @L_8022E1B0
@L_8022DFB4:
    cmpwi r0, 0x3d
    beq @L_8022E018
    b @L_8022E1B0
@L_8022DFC0:
    cmplwi r28, 0x2
    bne @L_8022E1B0
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022E1B0
    mr r3, r31
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r30
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_8037967E@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037967E@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E1B0
@L_8022E018:
    mr r3, r31
    bl fn_80203CCC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022E1B0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022E1B0
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    bl fn_80120DD0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r29
    bl fn_801252E0
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022E0BC
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022E0BC:
    lis r4, lbl_8037969F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037969F@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E1B0
@L_8022E0D4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xea
    li r6, 0x0
    bl fn_8012640C
    extsb r28, r3
    cmpwi r28, 0xc
    bge @L_8022E1B0
    mr r3, r31
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    beq @L_8022E1B0
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    addi r0, r28, 0x1
    mr r3, r31
    extsb r7, r0
    li r4, 0x0
    li r5, 0xea
    li r6, 0x0
    bl fn_801254B4
    lis r4, lbl_80379F58@ha
    lis r3, lbl_803796B5@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x11
    addis r5, r4, 0x1
    li r0, 0x0
    addi r4, r3, lbl_803796B5@l
    stb r6, 0x60a4(r5)
    lwz r3, lbl_8047B62C(r13)
    stb r0, 0x60a5(r5)
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E1B0
@L_8022E17C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    mr r3, r31
    cntlzw r0, r0
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    rlwinm r7, r0, 27, 24, 31
    bl fn_801254B4
@L_8022E1B0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
