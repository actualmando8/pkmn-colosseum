    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    li r5, 0xd6
    li r6, 0x0
    stw r0, 0x164(r1)
    stmw r29, 0x154(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FE1A0
    li r3, 0x0
    b @L_801FE1B0
    @L_801FE1A0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FE1B0:
    mr r4, r3
    addi r3, r1, 0x10
    bl fn_8011F5FC
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FE1F8
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FE1F8
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FE294
    @L_801FE1F8:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FE238
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FE268
    @L_801FE238:
    cmplwi r29, 0x0
    bne @L_801FE248
    li r3, 0x0
    b @L_801FE25C
    @L_801FE248:
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FE25C:
    li r4, 0x10
    bl fn_80121ADC
    b @L_801FE2BC
    @L_801FE268:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FE284
    li r3, 0x0
    b @L_801FE2BC
    @L_801FE284:
    mr r3, r29
    li r4, 0x10
    bl fn_8011B67C
    b @L_801FE2BC
    @L_801FE294:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FE2B0
    li r3, 0x0
    b @L_801FE2BC
    @L_801FE2B0:
    mr r3, r30
    li r4, 0x10
    bl fn_8011B67C
    @L_801FE2BC:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FE308
    mr r3, r30
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_801FDB78
    lwz r7, 0xc(r1)
    addi r3, r1, 0x10
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_801254B4
    lwz r7, 0x8(r1)
    addi r3, r1, 0x10
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_801254B4
    @L_801FE308:
    mr r4, r31
    addi r3, r1, 0x10
    bl fn_8011FDC8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FE338
    li r29, 0x0
    b @L_801FE34C
    @L_801FE338:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    @L_801FE34C:
    cmplwi r29, 0x0
    beq @L_801FE36C
    mr r3, r29
    mr r4, r31
    bl fn_801202CC
    mr r3, r29
    bl fn_8001D994
    stb r3, 0x28(r31)
    @L_801FE36C:
    li r0, 0x1
    mr r4, r30
    stb r0, 0x16(r31)
    li r3, 0x0
    bl fn_801F4354
    cmplwi r3, 0x0
    bne @L_801FE390
    li r0, 0x0
    b @L_801FE3AC
    @L_801FE390:
    bl fn_801FB8F8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FE3A8
    li r0, 0x1
    b @L_801FE3AC
    @L_801FE3A8:
    li r0, 0x0
    @L_801FE3AC:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801FE3C0
    li r0, 0x0
    stb r0, 0x16(r31)
    @L_801FE3C0:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_801F54A4
    clrlwi. r0, r3, 24
    bne @L_801FE3E4
    li r0, 0x1
    stb r0, 0x16(r31)
    @L_801FE3E4:
    lmw r29, 0x154(r1)
    lwz r0, 0x164(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr
