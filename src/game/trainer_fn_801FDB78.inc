    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r31, r3
    mr r28, r4
    mr r27, r5
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDBD0
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FDBD0
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FDC6C
    @L_801FDBD0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDC10
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FDC40
    @L_801FDC10:
    cmplwi r26, 0x0
    bne @L_801FDC20
    li r3, 0x0
    b @L_801FDC34
    @L_801FDC20:
    mr r3, r26
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FDC34:
    li r4, 0x10
    bl fn_80121ADC
    b @L_801FDC94
    @L_801FDC40:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FDC5C
    li r3, 0x0
    b @L_801FDC94
    @L_801FDC5C:
    mr r3, r26
    li r4, 0x10
    bl fn_8011B67C
    b @L_801FDC94
    @L_801FDC6C:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FDC88
    li r3, 0x0
    b @L_801FDC94
    @L_801FDC88:
    mr r3, r31
    li r4, 0x10
    bl fn_8011B67C
    @L_801FDC94:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801FE0C0
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDCDC
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FDCDC
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FDD78
    @L_801FDCDC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDD1C
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FDD4C
    @L_801FDD1C:
    cmplwi r26, 0x0
    bne @L_801FDD2C
    li r3, 0x0
    b @L_801FDD40
    @L_801FDD2C:
    mr r3, r26
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FDD40:
    li r4, 0x10
    bl fn_80121574
    b @L_801FDDA0
    @L_801FDD4C:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FDD68
    li r3, 0x0
    b @L_801FDDA0
    @L_801FDD68:
    mr r3, r26
    li r4, 0x10
    bl fn_8011A3E4
    b @L_801FDDA0
    @L_801FDD78:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FDD94
    li r3, 0x0
    b @L_801FDDA0
    @L_801FDD94:
    mr r3, r31
    li r4, 0x10
    bl fn_8011A3E4
    @L_801FDDA0:
    mr r30, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDDE0
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FDDE0
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FDE7C
    @L_801FDDE0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDE20
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FDE50
    @L_801FDE20:
    cmplwi r26, 0x0
    bne @L_801FDE30
    li r3, 0x0
    b @L_801FDE44
    @L_801FDE30:
    mr r3, r26
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FDE44:
    li r4, 0x10
    bl fn_8012189C
    b @L_801FDEA4
    @L_801FDE50:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FDE6C
    li r3, -0x1
    b @L_801FDEA4
    @L_801FDE6C:
    mr r3, r26
    li r4, 0x10
    bl fn_8011AE40
    b @L_801FDEA4
    @L_801FDE7C:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FDE98
    li r3, -0x1
    b @L_801FDEA4
    @L_801FDE98:
    mr r3, r31
    li r4, 0x10
    bl fn_8011AE40
    @L_801FDEA4:
    clrlwi r29, r3, 24
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDEE4
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FDEE4
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FDF80
    @L_801FDEE4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDF24
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FDF54
    @L_801FDF24:
    cmplwi r26, 0x0
    bne @L_801FDF34
    li r3, 0x0
    b @L_801FDF48
    @L_801FDF34:
    mr r3, r26
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FDF48:
    li r4, 0x10
    bl fn_8012182C
    b @L_801FDFA8
    @L_801FDF54:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FDF70
    li r3, -0x1
    b @L_801FDFA8
    @L_801FDF70:
    mr r3, r26
    li r4, 0x10
    bl fn_8011ACB4
    b @L_801FDFA8
    @L_801FDF80:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FDF9C
    li r3, -0x1
    b @L_801FDFA8
    @L_801FDF9C:
    mr r3, r31
    li r4, 0x10
    bl fn_8011ACB4
    @L_801FDFA8:
    clrlwi r26, r3, 24
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FDFE8
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_801FDFE8
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_801FE084
    @L_801FDFE8:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_801FE028
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_801FE058
    @L_801FE028:
    cmplwi r31, 0x0
    bne @L_801FE038
    li r3, 0x0
    b @L_801FE04C
    @L_801FE038:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FE04C:
    li r4, 0x10
    bl fn_8012165C
    b @L_801FE0AC
    @L_801FE058:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    beq @L_801FE074
    li r3, 0x0
    b @L_801FE0AC
    @L_801FE074:
    mr r3, r31
    li r4, 0x10
    bl fn_8011A6D4
    b @L_801FE0AC
    @L_801FE084:
    li r3, 0x10
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    beq @L_801FE0A0
    li r3, 0x0
    b @L_801FE0AC
    @L_801FE0A0:
    mr r3, r31
    li r4, 0x10
    bl fn_8011A6D4
    @L_801FE0AC:
    clrlslwi r0, r26, 24, 8
    rlwimi r0, r29, 0, 24, 31
    mr r26, r0
    rlwimi r26, r3, 16, 0, 15
    b @L_801FE13C
    @L_801FE0C0:
    cmplwi r31, 0x0
    bne @L_801FE0D0
    li r26, 0x0
    b @L_801FE108
    @L_801FE0D0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FE0F4
    li r3, 0x0
    b @L_801FE104
    @L_801FE0F4:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FE104:
    mr r26, r3
    @L_801FE108:
    mr r3, r26
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r26
    mr r26, r0
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    @L_801FE13C:
    cmplwi r28, 0x0
    beq @L_801FE148
    stw r30, 0x0(r28)
    @L_801FE148:
    cmplwi r27, 0x0
    beq @L_801FE154
    stw r26, 0x0(r27)
    @L_801FE154:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
