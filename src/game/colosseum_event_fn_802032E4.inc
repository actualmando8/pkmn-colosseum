    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    mr r31, r4
    bne @L_80203308
    li r30, 0x0
    b @L_8020331C
@L_80203308:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
@L_8020331C:
    cmplwi r30, 0x0
    beq @L_80203408
    cmplwi r28, 0x0
    bne @L_80203334
    li r29, 0x0
    b @L_8020334C
@L_80203334:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
@L_8020334C:
    cmplwi r29, 0x0
    bne @L_8020335C
    li r4, 0x0
    b @L_802033FC
@L_8020335C:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80203384
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_802033B4
@L_80203384:
    cmplwi r28, 0x0
    bne @L_80203394
    li r3, 0x0
    b @L_802033A8
@L_80203394:
    mr r3, r28
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802033A8:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_802033DC
@L_802033B4:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_802033D0
    li r3, 0x0
    b @L_802033DC
@L_802033D0:
    mr r3, r28
    li r4, 0x3d
    bl fn_8011B67C
@L_802033DC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802033F0
    li r4, 0x0
    b @L_802033FC
@L_802033F0:
    mr r3, r29
    bl fn_80123090
    mr r4, r3
@L_802033FC:
    mr r3, r30
    mr r5, r31
    bl fn_80122370
@L_80203408:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
