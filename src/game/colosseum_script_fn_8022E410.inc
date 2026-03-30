    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    beq @L_8022E6DC
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022E6DC
    mr r3, r31
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x2d
    beq @L_8022E4F8
    bge @L_8022E47C
    cmpwi r0, 0x16
    beq @L_8022E5A8
    bge @L_8022E470
    cmpwi r0, 0xd
    beq @L_8022E6BC
    bge @L_8022E6DC
    cmpwi r0, 0x2
    beq @L_8022E4A0
    b @L_8022E6DC
@L_8022E470:
    cmpwi r0, 0x24
    beq @L_8022E608
    b @L_8022E6DC
@L_8022E47C:
    cmpwi r0, 0x46
    beq @L_8022E550
    bge @L_8022E494
    cmpwi r0, 0x3b
    beq @L_8022E668
    b @L_8022E6DC
@L_8022E494:
    cmpwi r0, 0x4d
    beq @L_8022E6BC
    b @L_8022E6DC
@L_8022E4A0:
    li r3, 0x0
    li r4, 0x50
    bl fn_801F2988
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022E6DC
    li r3, 0x0
    li r4, 0x50
    li r5, 0x0
    bl fn_801F2934
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_8037959E@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037959E@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E6DC
@L_8022E4F8:
    li r3, 0x0
    li r4, 0x51
    bl fn_801F2988
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022E6DC
    li r3, 0x0
    li r4, 0x51
    li r5, 0x0
    bl fn_801F2934
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803795BB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803795BB@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E6DC
@L_8022E550:
    li r3, 0x0
    li r4, 0x4f
    bl fn_801F2988
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022E6DC
    li r3, 0x0
    li r4, 0x4f
    li r5, 0x0
    bl fn_801F2934
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803795D8@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803795D8@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E6DC
@L_8022E5A8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x116
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_8022E6DC
    mr r3, r31
    li r4, 0x3b
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022E5EC
    mr r3, r31
    li r4, 0x3b
    li r5, 0x0
    bl fn_8020248C
@L_8022E5EC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x116
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8022E6DC
@L_8022E608:
    mr r3, r31
    li r4, 0x0
    li r5, 0x117
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_8022E6DC
    mr r3, r31
    li r4, 0x3c
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022E64C
    mr r3, r31
    li r4, 0x3c
    li r5, 0x0
    bl fn_8020248C
@L_8022E64C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x117
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8022E6DC
@L_8022E668:
    mr r3, r31
    bl fn_8022EC40
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022E6DC
    lis r4, lbl_80379F58@ha
    subi r0, r3, 0x1
    addi r3, r4, lbl_80379F58@l
    mr r7, r31
    addis r3, r3, 0x1
    li r4, 0x0
    stb r0, 0x609b(r3)
    li r3, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803796D7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803796D7@l
    li r5, 0x0
    bl fn_80211B94
    b @L_8022E6DC
@L_8022E6BC:
    li r0, 0x0
    lis r3, -32733
    stb r0, 0x8(r1)
    addi r4, r3, -5220
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
@L_8022E6DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
