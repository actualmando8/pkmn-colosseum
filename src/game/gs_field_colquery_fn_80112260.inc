    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r23, r3
    li r28, 0x0
    mr r29, r28
    bl fn_80115BD8
    bl fn_80115B48
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_8011236C
    bl fn_80115BD8
    bl fn_80115A80
    mr r4, r27
    mr r26, r3
    bl fn_800F9318
    lis r4, lbl_802720B0@ha
    addi r4, r4, lbl_802720B0@l
    bl fn_80191ECC
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_8011236C
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_80112314
    mr r3, r27
    bl fn_80115274
    mr r31, r3
    clrlwi r24, r23, 24
    li r25, 0x0
    b @L_80112304
@L_801122E0:
    mr r3, r26
    or r4, r31, r28
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801122FC
    mr r4, r24
    bl fn_800E4014
@L_801122FC:
    addi r25, r25, 0x4
    addi r28, r28, 0x1
@L_80112304:
    lwz r3, 0x0(r30)
    lwzx r0, r3, r25
    cmplwi r0, 0x0
    bne @L_801122E0
@L_80112314:
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_8011236C
    mr r3, r27
    bl fn_80115268
    mr r31, r3
    clrlwi r25, r23, 24
    li r24, 0x0
    b @L_8011235C
@L_80112338:
    mr r3, r26
    or r4, r31, r29
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80112354
    mr r4, r25
    bl fn_800DCC34
@L_80112354:
    addi r24, r24, 0x4
    addi r29, r29, 0x1
@L_8011235C:
    lwz r3, 0x8(r30)
    lwzx r0, r3, r24
    cmplwi r0, 0x0
    bne @L_80112338
@L_8011236C:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
