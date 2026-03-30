    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r25, 0x44(r1)
    mr r27, r4
    mr r26, r3
    mr r28, r5
    li r30, 0x0
    mr r3, r27
    li r29, 0x0
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F205C
    li r3, 0x0
    b @L_801F2208
    @L_801F205C:
    li r5, 0x17
    li r6, 0x0
    li r0, 0x2
    lis r3, fn_801F34EC@ha
    stw r5, 0x28(r1)
    addi r4, r3, fn_801F34EC@l
    mr r3, r26
    addi r5, r1, 0x28
    stw r6, 0x2c(r1)
    li r6, 0x0
    stw r0, 0x30(r1)
    stw r27, 0x34(r1)
    bl fn_801F37B0
    li r6, 0x47
    li r5, 0x0
    li r0, 0x2
    lis r3, fn_801F34EC@ha
    stw r6, 0x18(r1)
    addi r4, r3, fn_801F34EC@l
    lwz r31, 0x2c(r1)
    mr r3, r26
    stw r5, 0x1c(r1)
    addi r5, r1, 0x18
    li r6, 0x0
    stw r0, 0x20(r1)
    stw r27, 0x24(r1)
    bl fn_801F37B0
    li r0, 0x0
    li r4, 0x2a
    lis r3, fn_801F34EC@ha
    stw r4, 0x8(r1)
    addi r4, r3, fn_801F34EC@l
    lwz r25, 0x1c(r1)
    stw r0, 0xc(r1)
    mr r3, r26
    addi r5, r1, 0x8
    li r6, 0x0
    stw r0, 0x10(r1)
    stw r27, 0x14(r1)
    bl fn_801F37B0
    lwz r26, 0xc(r1)
    mr r3, r27
    li r4, 0x2
    bl fn_80207AE0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_801F212C
    mr r3, r27
    bl fn_80207BF4
    clrlwi r0, r3, 16
    cmplwi r0, 0x1a
    bne @L_801F2130
    @L_801F212C:
    li r30, 0x1
    @L_801F2130:
    mr r3, r27
    li r4, 0x8
    bl fn_80207AE0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F214C
    li r29, 0x1
    @L_801F214C:
    mr r3, r27
    li r4, 0x16
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_801F2194
    mr r3, r27
    li r4, 0xe
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_801F2194
    mr r3, r27
    li r4, 0x25
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F219C
    @L_801F2194:
    li r3, 0x1
    b @L_801F2208
    @L_801F219C:
    cmplwi r31, 0x0
    beq @L_801F21B8
    cmplwi r28, 0x0
    beq @L_801F21B0
    stw r31, 0x0(r28)
    @L_801F21B0:
    li r3, 0x2
    b @L_801F2208
    @L_801F21B8:
    cmplwi r25, 0x0
    beq @L_801F21DC
    clrlwi. r0, r30, 24
    bne @L_801F21DC
    cmplwi r28, 0x0
    beq @L_801F21D4
    stw r25, 0x0(r28)
    @L_801F21D4:
    li r3, 0x2
    b @L_801F2208
    @L_801F21DC:
    cmplwi r26, 0x0
    beq @L_801F2204
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    bne @L_801F2204
    cmplwi r28, 0x0
    beq @L_801F21FC
    stw r26, 0x0(r28)
    @L_801F21FC:
    li r3, 0x2
    b @L_801F2208
    @L_801F2204:
    li r3, 0x0
    @L_801F2208:
    lmw r25, 0x44(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
