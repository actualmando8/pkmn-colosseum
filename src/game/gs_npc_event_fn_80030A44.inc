    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r4
    lwz r4, 0x64(r30)
    li r29, 0x0
    lbz r0, 0x8b(r3)
    lis r3, lbl_80266F68@ha
    clrrwi r4, r4, 8
    mr r5, r29
    or r31, r4, r0
    addi r4, r3, lbl_80266F68@l
    lha r3, 0x6(r30)
    lhz r0, 0x4(r4)
    cmpw r3, r0
    bne @L_80030A94
    lhz r5, 0x0(r4)
    @L_80030A94:
    addi r4, r4, 0x14
    lha r3, 0x6(r30)
    lhz r0, 0x4(r4)
    cmpw r3, r0
    bne @L_80030AAC
    lhz r5, 0x0(r4)
    @L_80030AAC:
    cmpwi r5, 0x2
    beq @L_80030AE0
    bge @L_80030AFC
    cmpwi r5, 0x1
    bge @L_80030AC4
    b @L_80030AFC
    @L_80030AC4:
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r29, r3
    b @L_80030AFC
    @L_80030AE0:
    lwz r0, lbl_8047A420(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r29, r3
    @L_80030AFC:
    cmplwi r29, 0x0
    beq @L_80030BEC
    mr r3, r29
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80030BF8
    mr r3, r29
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80030BF8
    mr r3, r29
    bl fn_8001DA60
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_80030B98
    bge @L_80030B88
    cmpwi r0, 0x0
    bge @L_80030B90
    b @L_80030BA0
    @L_80030B88:
    cmpwi r0, 0x3
    b @L_80030BA0
    @L_80030B90:
    li r29, 0xd67
    b @L_80030BA4
    @L_80030B98:
    li r29, 0xd68
    b @L_80030BA4
    @L_80030BA0:
    li r29, 0x0
    @L_80030BA4:
    cmplwi r29, 0x0
    beq @L_80030BF8
    li r3, 0xe7
    bl fn_800FA444
    srwi r0, r3, 16
    mr r3, r29
    extsh r0, r0
    clrlwi r29, r0, 16
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r5, r31
    addi r3, r29, 0x2
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_80030BF8
    @L_80030BEC:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    @L_80030BF8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
