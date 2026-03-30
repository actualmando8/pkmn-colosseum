    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    stw r0, 0x164(r1)
    stmw r28, 0x150(r1)
    mr. r30, r3
    mr r28, r4
    bne @L_80207C90
    li r3, 0x0
    b @L_80207CC0
@L_80207C90:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80207CB0
    li r3, 0x0
    b @L_80207CC0
@L_80207CB0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80207CC0:
    mr r4, r3
    addi r3, r1, 0x10
    bl fn_8011F5FC
    addi r3, r1, 0x10
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r29
    li r5, 0x66
    li r6, 0x0
    bl fn_8012640C
    cmplwi r29, 0x181
    mr r31, r3
    bne @L_80207D50
    rlwinm r0, r28, 0, 16, 31
    cmpwi r0, 0x3
    beq @L_80207D48
    bge @L_80207D24
    cmpwi r0, 0x1
    beq @L_80207D30
    bge @L_80207D38
    b @L_80207D48
@L_80207D24:
    cmpwi r0, 0x5
    bge @L_80207D48
    b @L_80207D40
@L_80207D30:
    li r0, 0x19f
    b @L_80207D4C
@L_80207D38:
    li r0, 0x19e
    b @L_80207D4C
@L_80207D40:
    li r0, 0x1a0
    b @L_80207D4C
@L_80207D48:
    li r0, 0x181
@L_80207D4C:
    mr r31, r0
@L_80207D50:
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80207D8C
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_80207D8C
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80207E28
@L_80207D8C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80207DCC
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80207DFC
@L_80207DCC:
    cmplwi r29, 0x0
    bne @L_80207DDC
    li r3, 0x0
    b @L_80207DF0
@L_80207DDC:
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80207DF0:
    li r4, 0x14
    bl fn_80121ADC
    b @L_80207E50
@L_80207DFC:
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80207E18
    li r3, 0x0
    b @L_80207E50
@L_80207E18:
    mr r3, r29
    li r4, 0x14
    bl fn_8011B67C
    b @L_80207E50
@L_80207E28:
    li r3, 0x14
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_80207E44
    li r3, 0x0
    b @L_80207E50
@L_80207E44:
    mr r3, r30
    li r4, 0x14
    bl fn_8011B67C
@L_80207E50:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80207E60
    li r31, 0x19d
@L_80207E60:
    cmplwi r31, 0x0
    bne @L_80207E70
    li r3, 0x0
    b @L_80207F48
@L_80207E70:
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
    addi r3, r1, 0x10
    bl fn_80125390
    lwz r4, 0xc(r1)
    mr r5, r3
    rlwinm r3, r31, 0, 16, 31
    bl fn_801DE190
    mr r0, r3
    addi r3, r1, 0x10
    mr r29, r0
    mr r4, r29
    bl fn_80122040
    cmplwi r30, 0x0
    bne @L_80207EEC
    li r3, 0x0
    b @L_80207F20
@L_80207EEC:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80207F10
    li r3, 0x0
    b @L_80207F20
@L_80207F10:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80207F20:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_80142CF4
    mr r3, r29
@L_80207F48:
    lmw r28, 0x150(r1)
    lwz r0, 0x164(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr
