    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r31, r3
    bne @L_801FEF94
    li r3, 0x0
    b @L_801FF1A8
    @L_801FEF94:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FEFD0
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FEFD0
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FF06C
    @L_801FEFD0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FF010
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FF040
    @L_801FF010:
    cmplwi r30, 0x0
    bne @L_801FF020
    li r3, 0x0
    b @L_801FF034
    @L_801FF020:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FF034:
    li r4, 0x14
    bl fn_80121ADC
    b @L_801FF094
    @L_801FF040:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FF05C
    li r3, 0x0
    b @L_801FF094
    @L_801FF05C:
    mr r3, r30
    li r4, 0x14
    bl fn_8011B67C
    b @L_801FF094
    @L_801FF06C:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FF088
    li r3, 0x0
    b @L_801FF094
    @L_801FF088:
    mr r3, r31
    li r4, 0x14
    bl fn_8011B67C
    @L_801FF094:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FF1A4
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FF0DC
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FF0DC
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FF178
    @L_801FF0DC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FF11C
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FF14C
    @L_801FF11C:
    cmplwi r30, 0x0
    bne @L_801FF12C
    li r3, 0x0
    b @L_801FF140
    @L_801FF12C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FF140:
    li r4, 0x14
    bl fn_80121574
    b @L_801FF1A8
    @L_801FF14C:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FF168
    li r3, 0x0
    b @L_801FF1A8
    @L_801FF168:
    mr r3, r30
    li r4, 0x14
    bl fn_8011A3E4
    b @L_801FF1A8
    @L_801FF178:
    li r3, 0x14
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FF194
    li r3, 0x0
    b @L_801FF1A8
    @L_801FF194:
    mr r3, r31
    li r4, 0x14
    bl fn_8011A3E4
    b @L_801FF1A8
    @L_801FF1A4:
    li r3, 0x0
    @L_801FF1A8:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
