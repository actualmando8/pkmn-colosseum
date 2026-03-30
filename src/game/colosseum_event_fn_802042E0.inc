    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    bne @L_80204300
    li r31, 0x0
    b @L_80204314
@L_80204300:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80204314:
    cmplwi r31, 0x0
    bne @L_80204324
    li r3, 0x0
    b @L_802043C0
@L_80204324:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_8020434C
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_8020437C
@L_8020434C:
    cmplwi r30, 0x0
    bne @L_8020435C
    li r3, 0x0
    b @L_80204370
@L_8020435C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80204370:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_802043A4
@L_8020437C:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80204398
    li r3, 0x0
    b @L_802043A4
@L_80204398:
    mr r3, r30
    li r4, 0x3d
    bl fn_8011B67C
@L_802043A4:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802043B8
    li r3, 0x0
    b @L_802043C0
@L_802043B8:
    mr r3, r31
    bl fn_801230E0
@L_802043C0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
