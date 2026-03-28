    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_8006566C
    bge @L_80065674
    cmpwi r3, 0x0
    bge @L_80065664
    b @L_80065674
@L_80065664:
    li r31, 0x3
    b @L_80065678
@L_8006566C:
    li r31, 0x3
    b @L_80065678
@L_80065674:
    li r31, 0x3
@L_80065678:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r0, 0x154(r3)
    cmpwi r0, 0x2
    beq @L_8006569C
    lbz r0, 0x4(r30)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r30)
@L_8006569C:
    mr r3, r29
    mr r4, r30
    li r5, 0x3
    bl fn_80065A48
    mr r3, r31
    bl fn_8025DA18
    clrlwi r3, r3, 16
    lha r0, 0x6(r30)
    mulli r4, r3, 0x3
    lis r3, lbl_802ED9F0@ha
    cmpwi r0, 0xbb1
    addi r3, r3, lbl_802ED9F0@l
    add r3, r3, r4
    beq @L_800656FC
    bge @L_800656F0
    cmpwi r0, 0xb92
    beq @L_800656FC
    bge @L_80065714
    cmpwi r0, 0xb73
    beq @L_800656FC
    b @L_80065714
@L_800656F0:
    cmpwi r0, 0xbd0
    beq @L_800656FC
    b @L_80065714
@L_800656FC:
    lbz r0, 0x0(r3)
    stb r0, 0x64(r30)
    lbz r0, 0x1(r3)
    stb r0, 0x65(r30)
    lbz r0, 0x2(r3)
    stb r0, 0x66(r30)
@L_80065714:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
