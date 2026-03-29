    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r20, 0x20(r1)
    li r25, 0x0
    li r24, 0x1
    bl fn_8003CF38
    lis r3, lbl_803A6748@ha
    lfs f1, lbl_8047BAC4(r2)
    addi r28, r3, lbl_803A6748@l
    li r0, 0x0
    lfs f0, lbl_8047BAC8(r2)
    li r3, 0x0
    stfs f1, 0x14(r28)
    li r4, 0x1
    stfs f1, 0x18(r28)
    stfs f1, 0x1c(r28)
    stfs f1, 0x20(r28)
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
    stw r0, 0x8(r28)
    stfs f1, 0x3c(r28)
    stb r0, 0x40(r28)
    stfs f1, 0x44(r28)
    stfs f0, 0x48(r28)
    bl fn_80135938
    lis r4, lbl_803A6610@ha
    mr r6, r3
    addi r3, r4, lbl_803A6610@l
    li r5, 0xa
    li r4, 0x1
    bl fn_801240C4
    lis r3, lbl_803A6610@ha
    li r4, -0x1
    addi r3, r3, lbl_803A6610@l
    li r5, -0x1
    li r6, 0x0
    li r7, 0x0
    bl fn_80124410
    lwz r0, lbl_8047A4DC(r13)
    lis r4, lbl_803A6748@ha
    lfs f0, lbl_8047BAC4(r2)
    addi r29, r4, lbl_803A6748@l
    cmpwi r0, 0xa
    stw r3, 0x98(r29)
    stfs f0, 0x28(r29)
    blt @L_8003C888
    li r0, 0xa
    stw r0, 0xc(r29)
    b @L_8003C88C
    @L_8003C888:
    stw r0, 0xc(r29)
    @L_8003C88C:
    lis r3, lbl_802EF0A8@ha
    lis r6, 0x4330
    addi r4, r3, lbl_802EF0A8@l
    lis r5, lbl_803A6748@ha
    lha r3, 0x509e(r4)
    addis r8, r4, 0x1
    lha r0, 0x50a0(r4)
    addi r30, r5, lbl_803A6748@l
    xoris r3, r3, 0x8000
    lwz r7, lbl_8047A4DC(r13)
    xoris r0, r0, 0x8000
    stw r3, 0xc(r1)
    addi r26, r30, 0x4c
    lfd f2, lbl_8047BAE0(r2)
    stw r6, 0x8(r1)
    mr r3, r26
    lha r4, 0x7ca6(r8)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    lha r5, 0x7ca8(r8)
    stw r6, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r7, 0x10(r30)
    fsubs f0, f0, f2
    stfs f1, 0x2c(r30)
    stfs f1, 0x34(r30)
    stfs f0, 0x30(r30)
    stfs f0, 0x38(r30)
    bl fn_8010A5BC
    lis r3, lbl_803A6748@ha
    addi r31, r3, lbl_803A6748@l
    @L_8003C90C:
    cmpwi r25, 0x4
    beq @L_8003CBE8
    bge @L_8003C93C
    cmpwi r25, 0x1
    beq @L_8003C974
    bge @L_8003C930
    cmpwi r25, 0x0
    bge @L_8003C954
    b @L_8003CD90
    @L_8003C930:
    cmpwi r25, 0x3
    bge @L_8003CBA8
    b @L_8003CB18
    @L_8003C93C:
    cmpwi r25, 0x64
    beq @L_8003CD14
    bge @L_8003CD90
    cmpwi r25, 0x6
    bge @L_8003CD90
    b @L_8003CC9C
    @L_8003C954:
    li r3, 0x2f
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x2e
    li r4, 0x0
    bl fn_8010264C
    li r25, 0x1
    b @L_8003CD90
    @L_8003C974:
    li r3, 0x30
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x2d
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x2b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x2c
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x32
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    blt @L_8003CB08
    lwz r0, 0x10(r30)
    cmpwi r0, 0x0
    beq @L_8003CB00
    li r25, 0x2
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lis r3, lbl_803A6748@ha
    lwz r5, lbl_8047A4D4(r13)
    addi r3, r3, lbl_803A6748@l
    lwz r27, lbl_8047A4D0(r13)
    lwz r3, 0x0(r3)
    clrlwi r0, r3, 16
    slwi r4, r3, 2
    slwi r3, r0, 2
    addi r0, r3, 0x2
    add r3, r5, r4
    lhzx r21, r5, r0
    lhz r22, 0x0(r3)
    lhz r20, 0x2(r3)
    mr r3, r21
    bl fn_801EE614
    mr r3, r21
    bl fn_801EE8F4
    mr r3, r21
    bl fn_801EEAD0
    mr r23, r3
    mr r3, r21
    bl fn_801EEC74
    clrlwi r0, r23, 24
    cmplwi r0, 0x0
    beq @L_8003CA58
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003CA50
    li r21, 0x1
    b @L_8003CA5C
    @L_8003CA50:
    li r21, 0x0
    b @L_8003CA5C
    @L_8003CA58:
    li r21, 0x2
    @L_8003CA5C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r27
    mr r4, r22
    li r5, 0xa
    bl fn_801240C4
    lwz r23, 0x98(r29)
    mr r3, r20
    bl fn_801EE750
    mr r3, r27
    mr r4, r23
    bl fn_8011DFE0
    cmpwi r21, 0x1
    beq @L_8003CAD0
    bge @L_8003CAAC
    cmpwi r21, 0x0
    bge @L_8003CAB4
    b @L_8003CAE8
    @L_8003CAAC:
    cmpwi r21, 0x3
    bge @L_8003CAE8
    @L_8003CAB4:
    mr r3, r27
    li r4, 0x1
    bl fn_8011D8F4
    mr r3, r27
    li r4, 0xa
    bl fn_8011D8D8
    b @L_8003CAE8
    @L_8003CAD0:
    mr r3, r27
    li r4, 0x0
    bl fn_8011D8F4
    mr r3, r27
    li r4, 0x0
    bl fn_8011D8D8
    @L_8003CAE8:
    lwz r4, lbl_8047A4D0(r13)
    mr r3, r26
    bl fn_80109C88
    lfs f0, lbl_8047BAC4(r2)
    stfs f0, 0x48(r28)
    b @L_8003CD90
    @L_8003CB00:
    li r25, 0x1
    b @L_8003CD90
    @L_8003CB08:
    lfs f0, lbl_8047BAC4(r2)
    li r25, 0x5
    stfs f0, 0x48(r28)
    b @L_8003CD90
    @L_8003CB18:
    lfs f0, 0x48(r28)
    lfs f2, 0x44(r28)
    fcmpu cr0, f0, f2
    beq @L_8003CB70
    lfs f1, 0x24(r31)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BAD4(r2)
    fdivs f0, f1, f0
    ble @L_8003CB58
    fadds f1, f2, f0
    lfs f0, lbl_8047BAC8(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    ble @L_8003CB70
    stfs f0, 0x44(r28)
    b @L_8003CB70
    @L_8003CB58:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BAC4(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    bge @L_8003CB70
    stfs f0, 0x44(r28)
    @L_8003CB70:
    lfs f1, 0x48(r28)
    lfs f0, 0x44(r28)
    fcmpu cr0, f1, f0
    bne @L_8003CBA0
    li r25, 0x3
    li r3, 0x2f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lfs f0, lbl_8047BAC8(r2)
    stfs f0, 0x48(r28)
    b @L_8003CD90
    @L_8003CBA0:
    bl fn_800F0308
    b @L_8003CD90
    @L_8003CBA8:
    li r3, 0x33
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x34
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    bge @L_8003CD90
    li r25, 0x4
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f0, lbl_8047BAC4(r2)
    stfs f0, 0x48(r28)
    b @L_8003CD90
    @L_8003CBE8:
    lfs f0, 0x48(r28)
    lfs f2, 0x44(r28)
    fcmpu cr0, f0, f2
    beq @L_8003CC40
    lfs f1, 0x24(r31)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BAD4(r2)
    fdivs f0, f1, f0
    ble @L_8003CC28
    fadds f1, f2, f0
    lfs f0, lbl_8047BAC8(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    ble @L_8003CC40
    stfs f0, 0x44(r28)
    b @L_8003CC40
    @L_8003CC28:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BAC4(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    bge @L_8003CC40
    stfs f0, 0x44(r28)
    @L_8003CC40:
    lfs f1, 0x48(r28)
    lfs f0, 0x44(r28)
    fcmpu cr0, f1, f0
    bne @L_8003CC94
    li r3, 0x33
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A6748@ha
    lwz r5, lbl_8047A4D4(r13)
    addi r3, r3, lbl_803A6748@l
    li r4, 0x0
    lwz r0, 0x0(r3)
    slwi r3, r0, 2
    addi r0, r3, 0x2
    lhzx r3, r5, r0
    bl fn_801EEDEC
    lfs f0, lbl_8047BAC8(r2)
    li r25, 0x0
    stfs f0, 0x48(r28)
    b @L_8003CD90
    @L_8003CC94:
    bl fn_800F0308
    b @L_8003CD90
    @L_8003CC9C:
    lfs f0, 0x48(r28)
    lfs f2, 0x44(r28)
    fcmpu cr0, f0, f2
    beq @L_8003CCF4
    lfs f1, 0x24(r31)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BAD4(r2)
    fdivs f0, f1, f0
    ble @L_8003CCDC
    fadds f1, f2, f0
    lfs f0, lbl_8047BAC8(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    ble @L_8003CCF4
    stfs f0, 0x44(r28)
    b @L_8003CCF4
    @L_8003CCDC:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BAC4(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r28)
    bge @L_8003CCF4
    stfs f0, 0x44(r28)
    @L_8003CCF4:
    lfs f1, 0x48(r28)
    lfs f0, 0x44(r28)
    fcmpu cr0, f1, f0
    bne @L_8003CD0C
    li r25, 0x64
    b @L_8003CD90
    @L_8003CD0C:
    bl fn_800F0308
    b @L_8003CD90
    @L_8003CD14:
    li r3, 0x30
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x2f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x2d
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x2b
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x2c
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x2e
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x32
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    bl fn_801069FC
    li r24, 0x0
    @L_8003CD90:
    cmpwi r24, 0x0
    bne @L_8003C90C
    mr r3, r26
    bl fn_8010A420
    lwz r3, lbl_8047A4D0(r13)
    cmplwi r3, 0x0
    beq @L_8003CDD4
    bl fn_800E202C
    mr r22, r3
    clrlwi r0, r22, 16
    cmplwi r0, 0x0
    beq @L_8003CDCC
    bl fn_800E24B0
    mr r3, r22
    bl fn_800E209C
    @L_8003CDCC:
    li r0, 0x0
    stw r0, lbl_8047A4D0(r13)
    @L_8003CDD4:
    lwz r3, lbl_8047A4D4(r13)
    cmplwi r3, 0x0
    beq @L_8003CE08
    bl fn_800E202C
    mr r22, r3
    clrlwi r0, r22, 16
    cmplwi r0, 0x0
    beq @L_8003CE00
    bl fn_800E24B0
    mr r3, r22
    bl fn_800E209C
    @L_8003CE00:
    li r0, 0x0
    stw r0, lbl_8047A4D4(r13)
    @L_8003CE08:
    lmw r20, 0x20(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
