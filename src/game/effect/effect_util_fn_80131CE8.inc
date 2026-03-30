    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r3
    mr r31, r4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r30, r3, 16
    mr r4, r24
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    mr r4, r24
    li r3, 0x0
    bl fn_801F4354
    li r26, 0x0
    mr r28, r3
    li r25, 0x0
    b @L_80131DB4
@L_80131D44:
    mr r3, r29
    mr r4, r25
    bl fn_801F7258
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_80131DB0
    bl fn_801FA524
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80131DB0
    clrlwi r0, r26, 16
    cmplwi r0, 0x0
    bne @L_80131D90
    mr r3, r27
    bl fn_801F8100
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    b @L_80131DAC
@L_80131D90:
    cmplwi r0, 0x1
    bne @L_80131DAC
    mr r3, r27
    bl fn_801F8100
    mr r4, r3
    li r3, 0x57
    bl fn_80132A38
@L_80131DAC:
    addi r26, r26, 0x1
@L_80131DB0:
    addi r25, r25, 0x1
@L_80131DB4:
    clrlwi r0, r25, 16
    cmplwi r0, 0x2
    blt @L_80131D44
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80131E6C
    cmplwi r28, 0x0
    beq @L_80131E6C
    clrlwi r0, r26, 16
    cmplwi r0, 0x1
    bgt @L_80131E34
    mr r3, r28
    bl fn_801F8100
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    bne @L_80131E14
    li r3, 0x7722
    bl fn_800FA280
    b @L_80131EF0
@L_80131E14:
    cmplwi r0, 0x1
    bne @L_80131E28
    li r3, 0x7725
    bl fn_800FA280
    b @L_80131EF0
@L_80131E28:
    li r3, 0x7727
    bl fn_800FA280
    b @L_80131EF0
@L_80131E34:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    bne @L_80131E4C
    li r3, 0x7724
    bl fn_800FA280
    b @L_80131EF0
@L_80131E4C:
    cmplwi r0, 0x1
    bne @L_80131E60
    li r3, 0x7726
    bl fn_800FA280
    b @L_80131EF0
@L_80131E60:
    li r3, 0x7728
    bl fn_800FA280
    b @L_80131EF0
@L_80131E6C:
    mr r3, r24
    mr r4, r30
    bl fn_801F0058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80131EBC
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    bne @L_80131E9C
    li r3, 0x768a
    bl fn_800FA280
    b @L_80131EF0
@L_80131E9C:
    cmplwi r0, 0x1
    bne @L_80131EB0
    li r3, 0x768c
    bl fn_800FA280
    b @L_80131EF0
@L_80131EB0:
    li r3, 0x7688
    bl fn_800FA280
    b @L_80131EF0
@L_80131EBC:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    bne @L_80131ED4
    li r3, 0x7689
    bl fn_800FA280
    b @L_80131EF0
@L_80131ED4:
    cmplwi r0, 0x1
    bne @L_80131EE8
    li r3, 0x768b
    bl fn_800FA280
    b @L_80131EF0
@L_80131EE8:
    li r3, 0x7687
    bl fn_800FA280
@L_80131EF0:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
