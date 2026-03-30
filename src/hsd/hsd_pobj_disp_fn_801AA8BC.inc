    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r6
    stw r28, 0x10(r1)
    mr r28, r4
    lhz r0, 0xc(r3)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_801AAB94
    lhz r0, 0xc(r31)
    lis r3, 0x1
    addi r3, r3, -0x8000
    rlwinm r0, r0, 0, 16, 17
    cmpw r0, r3
    beq @L_801AA954
    bge @L_801AA928
    cmpwi r0, 0x4000
    beq @L_801AA948
    bge @L_801AA964
    cmpwi r0, 0x0
    beq @L_801AA93C
    b @L_801AA964
@L_801AA928:
    lis r3, 0x1
    subi r3, r3, 0x4000
    cmpw r0, r3
    beq @L_801AAB94
    b @L_801AA964
@L_801AA93C:
    li r3, 0x0
    bl fn_801B2878
    b @L_801AA964
@L_801AA948:
    li r3, 0x1
    bl fn_801B2878
    b @L_801AA964
@L_801AA954:
    li r3, 0x2
    bl fn_801B2878
    b @L_801AA964
    b @L_801AAB94
@L_801AA964:
    lwz r6, 0x0(r31)
    mr r3, r31
    mr r4, r28
    mr r5, r30
    lwz r12, 0x40(r6)
    mr r6, r29
    mtctr r12
    bctrl
    lhz r0, 0xc(r31)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    bne @L_801AAACC
    lwz r30, 0x8(r31)
    b @L_801AA9E0
@L_801AA99C:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x1
    beq @L_801AA9DC
    lwz r0, 0x0(r30)
    cmpwi r0, 0x19
    beq @L_801AA9DC
    bge @L_801AA9C8
    cmpwi r0, 0xb
    bge @L_801AA9C8
    cmpwi r0, 0x9
    bge @L_801AA9DC
@L_801AA9C8:
    lhz r0, 0x12(r30)
    lwz r3, 0x0(r30)
    lwz r4, 0x14(r30)
    rlwinm r5, r0, 0, 24, 31
    bl fn_800B84E0
@L_801AA9DC:
    addi r30, r30, 0x18
@L_801AA9E0:
    lwz r0, 0x0(r30)
    cmpwi r0, 0xff
    bne @L_801AA99C
    li r0, 0x0
    stw r0, lbl_8047B2FC(r13)
    bl fn_800B7D3C
    lwz r30, 0x8(r31)
    b @L_801AAA8C
@L_801AAA00:
    lwz r0, 0x0(r30)
    cmpwi r0, 0xb
    bge @L_801AAA20
    cmpwi r0, 0x9
    bge @L_801AAA2C
    cmpwi r0, 0x0
    bge @L_801AAA54
    b @L_801AAA64
@L_801AAA20:
    cmpwi r0, 0x19
    beq @L_801AAA2C
    b @L_801AAA64
@L_801AAA2C:
    lwz r3, 0x0(r30)
    li r4, 0x1
    bl fn_800B7874
    lwz r4, 0x0(r30)
    li r3, 0x0
    lwz r5, 0x8(r30)
    li r6, 0x4
    li r7, 0x0
    bl fn_800B7D74
    b @L_801AAA88
@L_801AAA54:
    lwz r3, 0x0(r30)
    lwz r4, 0x4(r30)
    bl fn_800B7874
    b @L_801AAA88
@L_801AAA64:
    lwz r3, 0x0(r30)
    lwz r4, 0x4(r30)
    bl fn_800B7874
    lwz r4, 0x0(r30)
    li r3, 0x0
    lwz r5, 0x8(r30)
    lwz r6, 0xc(r30)
    lbz r7, 0x10(r30)
    bl fn_800B7D74
@L_801AAA88:
    addi r30, r30, 0x18
@L_801AAA8C:
    lwz r0, 0x0(r30)
    cmpwi r0, 0xff
    bne @L_801AAA00
    li r0, 0x0
    stw r0, lbl_8047B300(r13)
    lwz r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_801AAAC0
    lis r4, lbl_80274F04@ha
    addi r3, r2, -23016
    addi r5, r4, lbl_80274F04@l
    li r4, 0x781
    bl fn_80196E10
@L_801AAAC0:
    mr r3, r31
    bl fn_801AB67C
    b @L_801AAB94
@L_801AAACC:
    lwz r29, 0x8(r31)
    lwz r0, lbl_8047B2FC(r13)
    cmplw r0, r29
    beq @L_801AAB18
    mr r30, r29
    b @L_801AAB08
@L_801AAAE4:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x1
    beq @L_801AAB04
    lhz r0, 0x12(r30)
    lwz r3, 0x0(r30)
    lwz r4, 0x14(r30)
    rlwinm r5, r0, 0, 24, 31
    bl fn_800B84E0
@L_801AAB04:
    addi r30, r30, 0x18
@L_801AAB08:
    lwz r0, 0x0(r30)
    cmpwi r0, 0xff
    bne @L_801AAAE4
    stw r29, lbl_8047B2FC(r13)
@L_801AAB18:
    lwz r3, lbl_8047B300(r13)
    lwz r0, 0x8(r31)
    cmplw r3, r0
    beq @L_801AAB84
    bl fn_800B7D3C
    lwz r29, 0x8(r31)
    b @L_801AAB70
@L_801AAB34:
    lwz r3, 0x0(r29)
    lwz r4, 0x4(r29)
    bl fn_800B7874
    lwz r0, 0x0(r29)
    cmpwi r0, 0x9
    bge @L_801AAB54
    cmpwi r0, 0x0
    bge @L_801AAB6C
@L_801AAB54:
    lwz r4, 0x0(r29)
    li r3, 0x0
    lwz r5, 0x8(r29)
    lwz r6, 0xc(r29)
    lbz r7, 0x10(r29)
    bl fn_800B7D74
@L_801AAB6C:
    addi r29, r29, 0x18
@L_801AAB70:
    lwz r0, 0x0(r29)
    cmpwi r0, 0xff
    bne @L_801AAB34
    lwz r0, 0x8(r31)
    stw r0, lbl_8047B300(r13)
@L_801AAB84:
    lhz r0, 0xe(r31)
    lwz r3, 0x10(r31)
    rlwinm r4, r0, 5, 0, 26
    bl fn_800BD0FC
@L_801AAB94:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
