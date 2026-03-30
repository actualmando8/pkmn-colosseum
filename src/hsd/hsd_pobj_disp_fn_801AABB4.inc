    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stw r31, 0x7c(r1)
    mr r31, r5
    stw r30, 0x78(r1)
    stw r29, 0x74(r1)
    lhz r0, 0xc(r3)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    beq @L_801AAD4C
    bge @L_801AABF0
    cmpwi r0, 0x0
    beq @L_801AABFC
    b @L_801AAE8C
@L_801AABF0:
    cmpwi r0, 0x2000
    beq @L_801AAE84
    b @L_801AAE8C
@L_801AABFC:
    lwz r0, 0x14(r3)
    cmplwi r0, 0x0
    bne @L_801AAD40
    bl fn_8019F01C
    lis r4, lbl_80465678@ha
    mr r29, r3
    addi r5, r4, lbl_80465678@l
    lwz r0, 0x0(r5)
    lis r4, lbl_80465678@ha
    addi r3, r4, lbl_80465678@l
    cmplw r0, r29
    lwz r0, 0x4(r3)
    bne @L_801AAC38
    cmplwi r0, 0x1
    beq @L_801AAE8C
@L_801AAC38:
    li r3, 0x0
    bl fn_800BD554
    mr r3, r31
    li r4, 0x0
    bl fn_800BD4B4
    lis r3, lbl_8036CC40@ha
    li r30, 0x0
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AAC74
    ori r30, r30, 0x1
@L_801AAC74:
    li r3, 0x0
    li r4, 0x1
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AAC8C
    ori r30, r30, 0x3
@L_801AAC8C:
    li r3, 0x0
    li r4, 0x5
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AACA4
    ori r30, r30, 0x3
@L_801AACA4:
    li r3, 0x0
    li r4, 0x2
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AACBC
    ori r30, r30, 0x5
@L_801AACBC:
    rlwinm. r0, r30, 0, 31, 31
    beq @L_801AAE8C
    mr r3, r31
    addi r4, r1, 0x38
    bl fn_800A2FAC
    cmplwi r3, 0x0
    bne @L_801AACE4
    mr r3, r31
    addi r4, r1, 0x38
    bl fn_800A2D64
@L_801AACE4:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AAD10
    addi r3, r1, 0x38
    li r4, 0x0
    bl fn_800BD504
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AAD10:
    rlwinm. r0, r30, 0, 29, 30
    beq @L_801AAE8C
    addi r3, r1, 0x38
    li r4, 0x1e
    li r5, 0x0
    bl fn_800BD58C
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    b @L_801AAE8C
@L_801AAD40:
    mr r5, r31
    bl fn_801AB270
    b @L_801AAE8C
@L_801AAD4C:
    bl fn_8019F01C
    lis r4, lbl_80465678@ha
    mr r30, r3
    addi r5, r4, lbl_80465678@l
    lwz r0, 0x0(r5)
    lis r4, lbl_80465678@ha
    addi r3, r4, lbl_80465678@l
    cmplw r0, r30
    lwz r0, 0x4(r3)
    bne @L_801AAD7C
    cmplwi r0, 0x1
    beq @L_801AAE8C
@L_801AAD7C:
    li r3, 0x0
    bl fn_800BD554
    mr r3, r31
    li r4, 0x0
    bl fn_800BD4B4
    lis r3, lbl_8036CC40@ha
    li r29, 0x0
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AADB8
    ori r29, r29, 0x1
@L_801AADB8:
    li r3, 0x0
    li r4, 0x1
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AADD0
    ori r29, r29, 0x3
@L_801AADD0:
    li r3, 0x0
    li r4, 0x5
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AADE8
    ori r29, r29, 0x3
@L_801AADE8:
    li r3, 0x0
    li r4, 0x2
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AAE00
    ori r29, r29, 0x5
@L_801AAE00:
    rlwinm. r0, r29, 0, 31, 31
    beq @L_801AAE8C
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_800A2FAC
    cmplwi r3, 0x0
    bne @L_801AAE28
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_800A2D64
@L_801AAE28:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AAE54
    addi r3, r1, 0x8
    li r4, 0x0
    bl fn_800BD504
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AAE54:
    rlwinm. r0, r29, 0, 29, 30
    beq @L_801AAE8C
    addi r3, r1, 0x8
    li r4, 0x1e
    li r5, 0x0
    bl fn_800BD58C
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    b @L_801AAE8C
@L_801AAE84:
    mr r5, r31
    bl fn_801AAEA8
@L_801AAE8C:
    lwz r0, 0x84(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    lwz r29, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
