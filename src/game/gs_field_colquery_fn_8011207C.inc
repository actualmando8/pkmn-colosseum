    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    lis r5, 0x8d3e
    li r29, 0x0
    subi r0, r5, 0x34f7
    stw r3, 0x8(r1)
    mulhwu r0, r0, r4
    mr r30, r29
    srwi r0, r0, 6
    stw r0, 0xc(r1)
    bl fn_80115BD8
    bl fn_80115B48
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_8011224C
    bl fn_80115BD8
    bl fn_80115A80
    mr r4, r28
    mr r27, r3
    bl fn_800F9318
    lis r4, lbl_802720B0@ha
    addi r4, r4, lbl_802720B0@l
    bl fn_80191ECC
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_8011224C
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_8011219C
    mr r3, r28
    bl fn_80115274
    mr r26, r3
    li r24, 0x0
    b @L_8011218C
@L_8011210C:
    or r25, r26, r29
    mr r3, r27
    mr r4, r25
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80112184
    lwz r5, 0x8(r1)
    lwz r4, 0xc(r1)
    mtctr r4
    cmplwi r4, 0x0
    beq @L_80112184
@L_80112138:
    lwz r0, 0x0(r5)
    subi r4, r4, 0x1
    cmplw r25, r0
    bne @L_8011217C
    lwz r0, 0x4(r5)
    cmpwi r0, 0x2
    beq @L_80112170
    bge @L_80112184
    cmpwi r0, 0x1
    bge @L_80112164
    b @L_80112184
@L_80112164:
    addi r4, r5, 0x8
    bl fn_800E9B2C
    b @L_80112184
@L_80112170:
    addi r4, r5, 0x8
    bl fn_800DC878
    b @L_80112184
@L_8011217C:
    addi r5, r5, 0x74
    bdnz @L_80112138
@L_80112184:
    addi r24, r24, 0x4
    addi r29, r29, 0x1
@L_8011218C:
    lwz r3, 0x0(r31)
    lwzx r0, r3, r24
    cmplwi r0, 0x0
    bne @L_8011210C
@L_8011219C:
    lwz r0, 0x8(r31)
    cmplwi r0, 0x0
    beq @L_8011224C
    mr r3, r28
    bl fn_80115268
    mr r28, r3
    li r24, 0x0
    b @L_8011223C
@L_801121BC:
    or r25, r28, r30
    mr r3, r27
    mr r4, r25
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80112234
    lwz r5, 0x8(r1)
    lwz r4, 0xc(r1)
    mtctr r4
    cmplwi r4, 0x0
    beq @L_80112234
@L_801121E8:
    lwz r0, 0x0(r5)
    subi r4, r4, 0x1
    cmplw r25, r0
    bne @L_8011222C
    lwz r0, 0x4(r5)
    cmpwi r0, 0x2
    beq @L_80112220
    bge @L_80112234
    cmpwi r0, 0x1
    bge @L_80112214
    b @L_80112234
@L_80112214:
    addi r4, r5, 0x8
    bl fn_800E9B2C
    b @L_80112234
@L_80112220:
    addi r4, r5, 0x8
    bl fn_800DC878
    b @L_80112234
@L_8011222C:
    addi r5, r5, 0x74
    bdnz @L_801121E8
@L_80112234:
    addi r24, r24, 0x4
    addi r30, r30, 0x1
@L_8011223C:
    lwz r3, 0x8(r31)
    lwzx r0, r3, r24
    cmplwi r0, 0x0
    bne @L_801121BC
@L_8011224C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
