    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    bne @L_8020420C
    li r31, 0x0
    b @L_80204220
@L_8020420C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80204220:
    cmplwi r31, 0x0
    bne @L_80204230
    li r3, 0x0
    b @L_802042CC
@L_80204230:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80204258
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80204288
@L_80204258:
    cmplwi r30, 0x0
    bne @L_80204268
    li r3, 0x0
    b @L_8020427C
@L_80204268:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020427C:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_802042B0
@L_80204288:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_802042A4
    li r3, 0x0
    b @L_802042B0
@L_802042A4:
    mr r3, r30
    li r4, 0x3d
    bl fn_8011B67C
@L_802042B0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802042C4
    li r3, 0x0
    b @L_802042CC
@L_802042C4:
    mr r3, r31
    bl fn_80123090
@L_802042CC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
