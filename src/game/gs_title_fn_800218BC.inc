    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    lis r3, lbl_803A1B90@ha
    li r5, 0x0
    addi r3, r3, lbl_803A1B90@l
    lwz r4, 0x40(r3)
    mtctr r4
    cmpwi r4, 0x0
    ble @L_8002191C
    @L_80021904:
    lhz r0, 0x0(r3)
    cmplw r28, r0
    beq @L_8002191C
    addi r3, r3, 0x8
    addi r5, r5, 0x1
    bdnz @L_80021904
    @L_8002191C:
    cmpw r5, r4
    blt @L_80021948
    li r3, 0x2
    li r4, 0x426a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_80021A74
    @L_80021948:
    li r31, 0x0
    lfs f31, lbl_8047B8A0(r2)
    @L_80021950:
    mr r3, r31
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_80014118
    lwz r3, 0xc(r1)
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80021998
    lwz r3, 0xc(r1)
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80021998
    lwz r3, 0xc(r1)
    bl fn_8011FC14
    fcmpo cr0, f1, f31
    bgt @L_800219A4
    @L_80021998:
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_80021950
    @L_800219A4:
    cmpwi r31, 0x6
    blt @L_800219D0
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_80021A74
    @L_800219D0:
    mr r3, r28
    li r4, 0x1
    bl fn_800141BC
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_80021A48
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_80014118
    lwz r3, 0xc(r1)
    li r30, 0x0
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80021A24
    lwz r3, 0xc(r1)
    bl fn_8011FC14
    lfs f0, lbl_8047B8A0(r2)
    fcmpo cr0, f1, f0
    ble @L_80021A24
    li r30, 0x1
    @L_80021A24:
    cmpwi r30, 0x0
    bne @L_80021A48
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    @L_80021A48:
    mr r3, r31
    bl fn_80014198
    cmpwi r31, 0x0
    blt @L_80021A70
    cmpwi r30, 0x0
    beq @L_80021A70
    li r0, 0x1
    li r3, 0x2
    stw r0, 0x0(r29)
    b @L_80021A74
    @L_80021A70:
    li r3, 0x1
    @L_80021A74:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
