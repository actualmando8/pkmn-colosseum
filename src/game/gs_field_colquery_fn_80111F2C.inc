    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    li r26, 0x0
    stw r3, 0x8(r1)
    mr r31, r26
    bl fn_80115BD8
    bl fn_80115B48
    mr r24, r3
    cmplwi r24, 0x0
    beq @L_80112068
    bl fn_80115BD8
    bl fn_80115A80
    mr r4, r24
    mr r30, r3
    bl fn_800F9318
    lis r4, lbl_802720B0@ha
    addi r4, r4, lbl_802720B0@l
    bl fn_80191ECC
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_80112068
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    beq @L_80111FF8
    mr r3, r24
    bl fn_80115274
    mr r29, r3
    li r22, 0x0
    li r28, 0x1
    b @L_80111FE8
@L_80111FAC:
    or r25, r29, r26
    mr r3, r30
    mr r4, r25
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80111FE0
    lwz r23, 0x8(r1)
    stw r25, 0x0(r23)
    addi r4, r23, 0x8
    stw r28, 0x4(r23)
    bl fn_800E9C6C
    addi r0, r23, 0x74
    stw r0, 0x8(r1)
@L_80111FE0:
    addi r22, r22, 0x4
    addi r26, r26, 0x1
@L_80111FE8:
    lwz r3, 0x0(r27)
    lwzx r0, r3, r22
    cmplwi r0, 0x0
    bne @L_80111FAC
@L_80111FF8:
    lwz r0, 0x8(r27)
    cmplwi r0, 0x0
    beq @L_80112068
    mr r3, r24
    bl fn_80115268
    mr r28, r3
    li r22, 0x0
    li r29, 0x2
    b @L_80112058
@L_8011201C:
    or r24, r28, r31
    mr r3, r30
    mr r4, r24
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80112050
    lwz r23, 0x8(r1)
    stw r24, 0x0(r23)
    addi r4, r23, 0x8
    stw r29, 0x4(r23)
    bl fn_800DCA10
    addi r0, r23, 0x74
    stw r0, 0x8(r1)
@L_80112050:
    addi r22, r22, 0x4
    addi r31, r31, 0x1
@L_80112058:
    lwz r3, 0x8(r27)
    lwzx r0, r3, r22
    cmplwi r0, 0x0
    bne @L_8011201C
@L_80112068:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
