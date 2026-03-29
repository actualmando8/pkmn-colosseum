    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    stw r28, 0x40(r1)
    mr r30, r3
    lis r5, lbl_80266C30@ha
    mr r3, r4
    addi r29, r5, lbl_80266C30@l
    addi r4, r1, 0xc
    lwz r31, 0x0(r29)
    addi r5, r1, 0x8
    lwz r12, 0x4(r29)
    lwz r11, 0x8(r29)
    lwz r10, 0xc(r29)
    lwz r9, 0x10(r29)
    lwz r8, 0x14(r29)
    lwz r7, 0x18(r29)
    lwz r6, 0x1c(r29)
    lwz r0, 0x20(r29)
    stw r31, 0x10(r1)
    stw r12, 0x14(r1)
    stw r11, 0x18(r1)
    stw r10, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r8, 0x24(r1)
    stw r7, 0x28(r1)
    stw r6, 0x2c(r1)
    stw r0, 0x30(r1)
    bl fn_80014118
    lwz r3, 0xc(r1)
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80022BD8
    li r3, 0x1
    b @L_80022E34
    @L_80022BD8:
    li r29, 0x0
    clrlwi r3, r30, 16
    lhz r0, 0x10(r1)
    cmplw r3, r0
    beq @L_80022C10
    li r29, 0x1
    lhz r0, 0x1c(r1)
    cmplw r3, r0
    beq @L_80022C10
    li r29, 0x2
    lhz r0, 0x28(r1)
    cmplw r3, r0
    beq @L_80022C10
    li r29, 0x3
    @L_80022C10:
    cmpwi r29, 0x3
    blt @L_80022C20
    li r3, 0x1
    b @L_80022E34
    @L_80022C20:
    lwz r3, 0xc(r1)
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    mr r4, r30
    li r3, 0x2d
    bl fn_80132A38
    mulli r31, r29, 0xc
    addi r4, r1, 0x14
    li r3, 0x2
    li r5, 0x1
    lwzx r4, r4, r31
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    li r4, 0x9
    bl fn_80135168
    cmpwi r3, 0x0
    bne @L_80022CF4
    li r28, 0x0
    @L_80022C80:
    li r3, 0x1
    li r4, 0x0
    li r5, 0xff
    li r6, 0x15
    li r7, 0x0
    bl fn_800F78A4
    li r29, 0x0
    b @L_80022CAC
    @L_80022CA0:
    bl fn_800F0308
    bl fn_800D3088
    add r29, r29, r3
    @L_80022CAC:
    cmpwi r29, 0x3c
    blt @L_80022CA0
    addi r28, r28, 0x1
    cmpwi r28, 0x2
    blt @L_80022C80
    li r3, 0x1
    li r4, 0x0
    li r5, 0xff
    li r6, 0x30
    li r7, 0x0
    bl fn_800F78A4
    li r29, 0x0
    b @L_80022CEC
    @L_80022CE0:
    bl fn_800F0308
    bl fn_800D3088
    add r29, r29, r3
    @L_80022CEC:
    cmpwi r29, 0x60
    blt @L_80022CE0
    @L_80022CF4:
    lwz r29, 0xc(r1)
    cmplwi r29, 0x0
    bne @L_80022D08
    li r3, 0x0
    b @L_80022D30
    @L_80022D08:
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80022D24
    li r3, 0x0
    b @L_80022D30
    @L_80022D24:
    mr r3, r29
    bl fn_8011F5C8
    bl fn_8011E778
    @L_80022D30:
    cmplwi r3, 0x0
    bne @L_80022D40
    li r29, 0x0
    b @L_80022D58
    @L_80022D40:
    bl fn_8011E15C
    clrlwi r29, r3, 16
    mr r3, r29
    bl fn_80166A28
    b @L_80022D58
    @L_80022D54:
    bl fn_800F0308
    @L_80022D58:
    mr r3, r29
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80022D54
    lwz r3, 0xc(r1)
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x3af7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    lwz r3, 0xc(r1)
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80022DD8
    lwz r3, 0xc(r1)
    li r4, 0x3e
    bl fn_80121B4C
    li r3, 0x2
    li r4, 0x4277
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    @L_80022DD8:
    lwz r3, 0xc(r1)
    bl fn_8011FC14
    lfs f0, lbl_8047B8A0(r2)
    fcmpo cr0, f1, f0
    ble @L_80022E30
    lwz r3, 0xc(r1)
    clrlwi r4, r30, 16
    li r5, 0x4
    bl fn_8011F910
    lwz r3, 0xc(r1)
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    addi r4, r1, 0x18
    li r3, 0x2
    lwzx r4, r4, r31
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    @L_80022E30:
    li r3, 0x1
    @L_80022E34:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r28, 0x40(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
