    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r31, r3
    bne @L_801FFB50
    li r30, 0x0
    b @L_801FFC68
    @L_801FFB50:
    li r3, 0x0
    b @L_801FFB5C
    @L_801FFB58:
    addi r3, r3, 0x1
    @L_801FFB5C:
    clrlwi r0, r3, 24
    cmplwi r0, 0x4
    blt @L_801FFB58
    li r30, 0x0
    mr r28, r30
    b @L_801FFC5C
    @L_801FFB74:
    cmplwi r31, 0x0
    bne @L_801FFB84
    li r3, 0x0
    b @L_801FFBB8
    @L_801FFB84:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FFBA8
    li r3, 0x0
    b @L_801FFBB8
    @L_801FFBA8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FFBB8:
    clrlwi r29, r28, 24
    mr r4, r29
    bl fn_80123CD4
    clrlwi. r0, r3, 24
    beq @L_801FFC58
    cmplwi r31, 0x0
    bne @L_801FFBDC
    li r3, 0x0
    b @L_801FFC10
    @L_801FFBDC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FFC00
    li r3, 0x0
    b @L_801FFC10
    @L_801FFC00:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FFC10:
    mr r6, r29
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    clrlwi. r0, r3, 16
    beq @L_801FFC58
    cmplwi r0, 0x165
    beq @L_801FFC58
    cmplwi r0, 0x163
    beq @L_801FFC58
    mr r3, r31
    mr r4, r29
    li r5, 0x0
    li r6, 0x0
    bl fn_801FFEC8
    clrlwi. r0, r3, 24
    bne @L_801FFC58
    addi r30, r30, 0x1
    @L_801FFC58:
    addi r28, r28, 0x1
    @L_801FFC5C:
    clrlwi r0, r28, 24
    cmplwi r0, 0x4
    blt @L_801FFB74
    @L_801FFC68:
    clrlwi. r0, r30, 24
    bne @L_801FFC78
    li r3, 0x1
    b @L_801FFEB4
    @L_801FFC78:
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FFCB4
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FFCB4
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FFD50
    @L_801FFCB4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FFCF4
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FFD24
    @L_801FFCF4:
    cmplwi r30, 0x0
    bne @L_801FFD04
    li r3, 0x0
    b @L_801FFD18
    @L_801FFD04:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FFD18:
    li r4, 0x2a
    bl fn_80121ADC
    b @L_801FFD78
    @L_801FFD24:
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FFD40
    li r3, 0x0
    b @L_801FFD78
    @L_801FFD40:
    mr r3, r30
    li r4, 0x2a
    bl fn_8011B67C
    b @L_801FFD78
    @L_801FFD50:
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FFD6C
    li r3, 0x0
    b @L_801FFD78
    @L_801FFD6C:
    mr r3, r31
    li r4, 0x2a
    bl fn_8011B67C
    @L_801FFD78:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FFEB0
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FFDC0
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FFDC0
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FFE60
    @L_801FFDC0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FFE00
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FFE30
    @L_801FFE00:
    cmplwi r30, 0x0
    bne @L_801FFE10
    li r3, 0x0
    b @L_801FFE24
    @L_801FFE10:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FFE24:
    li r4, 0x2a
    bl fn_80121574
    b @L_801FFE58
    @L_801FFE30:
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FFE4C
    li r3, 0x0
    b @L_801FFE58
    @L_801FFE4C:
    mr r3, r30
    li r4, 0x2a
    bl fn_8011A3E4
    @L_801FFE58:
    mr r0, r3
    b @L_801FFE8C
    @L_801FFE60:
    li r3, 0x2a
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FFE7C
    li r0, 0x0
    b @L_801FFE8C
    @L_801FFE7C:
    mr r3, r31
    li r4, 0x2a
    bl fn_8011A3E4
    mr r0, r3
    @L_801FFE8C:
    mr r3, r31
    clrlwi r4, r0, 16
    li r5, 0x0
    li r6, 0x0
    bl fn_801FFEC8
    clrlwi. r0, r3, 24
    beq @L_801FFEB0
    li r3, 0x1
    b @L_801FFEB4
    @L_801FFEB0:
    li r3, 0x0
    @L_801FFEB4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
