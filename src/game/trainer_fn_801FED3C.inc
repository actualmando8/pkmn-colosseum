    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_801FEF60
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FED94
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FED94
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FEE30
    @L_801FED94:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FEDD4
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FEE04
    @L_801FEDD4:
    cmplwi r29, 0x0
    bne @L_801FEDE4
    li r3, 0x0
    b @L_801FEDF8
    @L_801FEDE4:
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FEDF8:
    li r4, 0x14
    bl fn_80121ADC
    b @L_801FEE58
    @L_801FEE04:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FEE20
    li r3, 0x0
    b @L_801FEE58
    @L_801FEE20:
    mr r3, r29
    li r4, 0x14
    bl fn_8011B67C
    b @L_801FEE58
    @L_801FEE30:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FEE4C
    li r3, 0x0
    b @L_801FEE58
    @L_801FEE4C:
    mr r3, r30
    li r4, 0x14
    bl fn_8011B67C
    @L_801FEE58:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FEF60
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FEEA0
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FEEA0
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FEF3C
    @L_801FEEA0:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FEEE0
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FEF14
    @L_801FEEE0:
    cmplwi r29, 0x0
    bne @L_801FEEF0
    li r3, 0x0
    b @L_801FEF04
    @L_801FEEF0:
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FEF04:
    mr r5, r31
    li r4, 0x14
    bl fn_801214FC
    b @L_801FEF60
    @L_801FEF14:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FEF60
    mr r3, r29
    mr r5, r31
    li r4, 0x14
    bl fn_8011A280
    b @L_801FEF60
    @L_801FEF3C:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    bne @L_801FEF60
    mr r3, r30
    mr r5, r31
    li r4, 0x14
    bl fn_8011A280
    @L_801FEF60:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
