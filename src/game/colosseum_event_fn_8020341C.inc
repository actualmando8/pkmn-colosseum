    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r30, r4
    mr r31, r5
    bne @L_80203444
    li r29, 0x0
    b @L_80203458
@L_80203444:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
@L_80203458:
    cmplwi r29, 0x0
    beq @L_80203548
    cmplwi r27, 0x0
    bne @L_80203470
    li r28, 0x0
    b @L_80203488
@L_80203470:
    mr r3, r27
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
@L_80203488:
    cmplwi r28, 0x0
    bne @L_80203498
    li r4, 0x0
    b @L_80203538
@L_80203498:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802034C0
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_802034F0
@L_802034C0:
    cmplwi r27, 0x0
    bne @L_802034D0
    li r3, 0x0
    b @L_802034E4
@L_802034D0:
    mr r3, r27
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802034E4:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_80203518
@L_802034F0:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_8020350C
    li r3, 0x0
    b @L_80203518
@L_8020350C:
    mr r3, r27
    li r4, 0x3d
    bl fn_8011B67C
@L_80203518:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020352C
    li r4, 0x0
    b @L_80203538
@L_8020352C:
    mr r3, r28
    bl fn_80123090
    mr r4, r3
@L_80203538:
    mr r3, r29
    mr r5, r30
    mr r6, r31
    bl fn_801226D0
@L_80203548:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
