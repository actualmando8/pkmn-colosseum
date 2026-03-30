    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r20, r3
    mr r31, r4
    addi r21, r1, 0x10
    li r24, 0x0
    b @L_8020AA64
@L_8020A904:
    clrlwi r29, r24, 24
    mr r3, r20
    mr r4, r29
    li r23, 0x0
    bl fn_8020A7F0
    mr r22, r3
    mr r3, r20
    mr r4, r29
    bl fn_8020A780
    mr r28, r3
    mr r3, r20
    mr r4, r29
    bl fn_8020A710
    mr r27, r3
    mr r3, r20
    mr r4, r29
    bl fn_8020A630
    mr r26, r3
    mr r3, r20
    mr r4, r29
    bl fn_8020A5C0
    mr r25, r3
    mr r3, r20
    mr r4, r29
    bl fn_8020A6A0
    clrlwi r0, r22, 24
    mr r22, r3
    cmpwi r0, 0x2
    beq @L_8020A9A0
    bge @L_8020A98C
    cmpwi r0, 0x0
    beq @L_8020AA34
    bge @L_8020A998
    b @L_8020AA34
@L_8020A98C:
    cmpwi r0, 0x4
    bge @L_8020AA34
    b @L_8020A9B8
@L_8020A998:
    clrlwi r23, r28, 16
    b @L_8020AA34
@L_8020A9A0:
    clrlwi r19, r28, 16
    clrlwi r0, r27, 16
    subf r3, r19, r0
    bl fn_800E0C04
    add r23, r19, r3
    b @L_8020AA34
@L_8020A9B8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r5, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F02AC
    mr. r23, r3
    bne @L_8020A9EC
    li r23, 0x0
    b @L_8020AA58
@L_8020A9EC:
    mr r3, r28
    bl fn_801F0234
    bl fn_801F021C
    clrlwi. r0, r22, 24
    bne @L_8020AA1C
    mr r4, r23
    mr r6, r27
    clrlwi r5, r26, 16
    clrlwi r7, r25, 16
    bl fn_80135E44
    mr r23, r3
    b @L_8020AA34
@L_8020AA1C:
    mr r4, r23
    mr r6, r27
    li r5, 0x0
    li r7, 0x0
    bl fn_80135E44
    mr r23, r3
@L_8020AA34:
    clrlwi r0, r22, 24
    cmplwi r0, 0x1
    bne @L_8020AA58
    extsh r3, r26
    extsh. r0, r25
    mullw r23, r23, r3
    beq @L_8020AA58
    extsh r0, r25
    divw r23, r23, r0
@L_8020AA58:
    clrlslwi r0, r24, 24, 2
    addi r24, r24, 0x1
    stwx r23, r21, r0
@L_8020AA64:
    clrlwi r0, r24, 24
    cmplwi r0, 0x2
    blt @L_8020A904
    mr r3, r20
    li r24, 0x0
    bl fn_8020A8A0
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    bgt @L_8020AB34
    lis r3, 0x8037
    slwi r0, r0, 2
    addi r3, r3, 0x5954
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    bne @L_8020AB34
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    beq @L_8020AB34
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    blt @L_8020AB34
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    bgt @L_8020AB34
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    bge @L_8020AB34
    li r24, 0x1
    b @L_8020AB34
    lwz r3, 0x10(r1)
    lwz r0, 0x14(r1)
    cmpw r3, r0
    ble @L_8020AB34
    li r24, 0x1
@L_8020AB34:
    mr r30, r24
    mr r3, r20
    bl fn_8020A860
    clrlwi. r0, r3, 16
    mr r28, r3
    bne @L_8020AB54
    mr r3, r24
    b @L_8020AE1C
@L_8020AB54:
    addi r27, r1, 0x8
@L_8020AB58:
    mr r3, r28
    bl fn_8020A540
    li r23, 0x0
    mr r29, r3
    b @L_8020ACCC
@L_8020AB6C:
    clrlwi r19, r23, 24
    mr r3, r29
    mr r4, r19
    li r20, 0x0
    bl fn_8020A7F0
    mr r26, r3
    mr r3, r29
    mr r4, r19
    bl fn_8020A780
    mr r21, r3
    mr r3, r29
    mr r4, r19
    bl fn_8020A710
    mr r22, r3
    mr r3, r29
    mr r4, r19
    bl fn_8020A630
    mr r24, r3
    mr r3, r29
    mr r4, r19
    bl fn_8020A5C0
    mr r25, r3
    mr r3, r29
    mr r4, r19
    bl fn_8020A6A0
    clrlwi r0, r26, 24
    mr r26, r3
    cmpwi r0, 0x2
    beq @L_8020AC08
    bge @L_8020ABF4
    cmpwi r0, 0x0
    beq @L_8020AC9C
    bge @L_8020AC00
    b @L_8020AC9C
@L_8020ABF4:
    cmpwi r0, 0x4
    bge @L_8020AC9C
    b @L_8020AC20
@L_8020AC00:
    clrlwi r20, r21, 16
    b @L_8020AC9C
@L_8020AC08:
    clrlwi r19, r21, 16
    clrlwi r0, r22, 16
    subf r3, r19, r0
    bl fn_800E0C04
    add r20, r19, r3
    b @L_8020AC9C
@L_8020AC20:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r5, r3, 16
    mr r3, r21
    mr r4, r31
    bl fn_801F02AC
    mr. r20, r3
    bne @L_8020AC54
    li r20, 0x0
    b @L_8020ACC0
@L_8020AC54:
    mr r3, r21
    bl fn_801F0234
    bl fn_801F021C
    clrlwi. r0, r26, 24
    bne @L_8020AC84
    mr r4, r20
    mr r6, r22
    clrlwi r5, r24, 16
    clrlwi r7, r25, 16
    bl fn_80135E44
    mr r20, r3
    b @L_8020AC9C
@L_8020AC84:
    mr r4, r20
    mr r6, r22
    li r5, 0x0
    li r7, 0x0
    bl fn_80135E44
    mr r20, r3
@L_8020AC9C:
    clrlwi r0, r26, 24
    cmplwi r0, 0x1
    bne @L_8020ACC0
    extsh r3, r24
    extsh. r0, r25
    mullw r20, r20, r3
    beq @L_8020ACC0
    extsh r0, r25
    divw r20, r20, r0
@L_8020ACC0:
    clrlslwi r0, r23, 24, 2
    addi r23, r23, 0x1
    stwx r20, r27, r0
@L_8020ACCC:
    clrlwi r0, r23, 24
    cmplwi r0, 0x2
    blt @L_8020AB6C
    mr r3, r29
    li r23, 0x0
    bl fn_8020A8A0
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    bgt @L_8020AD9C
    lis r3, 0x8037
    slwi r0, r0, 2
    addi r3, r3, 0x5938
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    bne @L_8020AD9C
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    beq @L_8020AD9C
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    blt @L_8020AD9C
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    bgt @L_8020AD9C
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    bge @L_8020AD9C
    li r23, 0x1
    b @L_8020AD9C
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    cmpw r3, r0
    ble @L_8020AD9C
    li r23, 0x1
@L_8020AD9C:
    mr r3, r29
    bl fn_8020A580
    clrlwi r0, r3, 24
    cmpwi r0, 0x2
    beq @L_8020ADE0
    bge @L_8020ADFC
    cmpwi r0, 0x1
    bge @L_8020ADC0
    b @L_8020ADFC
@L_8020ADC0:
    clrlwi. r0, r30, 24
    bne @L_8020ADD0
    clrlwi. r0, r23, 24
    beq @L_8020ADD8
@L_8020ADD0:
    li r30, 0x1
    b @L_8020ADFC
@L_8020ADD8:
    li r30, 0x0
    b @L_8020ADFC
@L_8020ADE0:
    clrlwi. r0, r30, 24
    beq @L_8020ADF8
    clrlwi. r0, r23, 24
    beq @L_8020ADF8
    li r30, 0x1
    b @L_8020ADFC
@L_8020ADF8:
    li r30, 0x0
@L_8020ADFC:
    mr r3, r28
    bl fn_8020A500
    clrlwi. r0, r3, 16
    mr r28, r3
    bne @L_8020AB58
    mr r3, r30
    b @L_8020AE1C
    b @L_8020AB54
@L_8020AE1C:
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
