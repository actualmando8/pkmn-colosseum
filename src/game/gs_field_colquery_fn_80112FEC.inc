    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r24, r3
    li r25, 0x0
    bl fn_80115A80
    li r29, 0x0
    mr r28, r3
    li r31, 0x0
    b @L_80113224
@L_80113018:
    lwz r3, lbl_80478EBC(r13)
    lwz r0, 0xc(r24)
    add r30, r3, r31
    lhz r3, 0x4(r30)
    cmplw r3, r0
    bne @L_8011321C
    lbz r0, 0x0(r30)
    rlwinm r0, r0, 27, 29, 31
    cmpwi r0, 0x2
    beq @L_80113068
    bge @L_80113050
    cmpwi r0, 0x1
    bge @L_8011305C
    b @L_8011307C
@L_80113050:
    cmpwi r0, 0x4
    bge @L_8011307C
    b @L_80113074
@L_8011305C:
    lis r3, 0x377
    addi r26, r3, 0x400
    b @L_8011307C
@L_80113068:
    lis r3, 0x378
    addi r26, r3, 0x400
    b @L_8011307C
@L_80113074:
    lis r3, 0x379
    addi r26, r3, 0x400
@L_8011307C:
    mr r0, r25
    addi r25, r25, 0x1
    oris r27, r0, 0x7fff
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801130AC
    mr r3, r28
    mr r4, r27
    mr r5, r26
    bl fn_8018E050
    b @L_801130B8
@L_801130AC:
    mr r3, r28
    mr r4, r27
    bl fn_8018D998
@L_801130B8:
    cmplwi r3, 0x0
    beq @L_8011321C
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8011321C
    lfs f1, 0x10(r30)
    addi r3, r1, 0x14
    lfs f2, 0x14(r30)
    lfs f3, 0x18(r30)
    bl fn_800E01F4
    mr r3, r28
    mr r4, r27
    addi r5, r1, 0x14
    bl fn_8018C0A8
    lha r4, 0x2(r30)
    lis r0, 0x4330
    lfs f1, -26416(r2)
    addi r3, r1, 0x8
    xoris r4, r4, 0x8000
    stw r0, 0x20(r1)
    lfd f2, -26400(r2)
    fmr f3, f1
    stw r4, 0x24(r1)
    lfs f4, -26412(r2)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f2
    fmuls f2, f4, f0
    bl fn_800E01F4
    mr r3, r28
    mr r4, r27
    addi r5, r1, 0x8
    bl fn_8018BF24
    mr r3, r28
    mr r4, r27
    bl fn_8018CB5C
    mr r3, r28
    mr r4, r27
    li r5, 0x4
    bl fn_8018C7C8
    mr r3, r28
    mr r4, r27
    li r5, 0x1
    bl fn_8018C1E8
    lbz r0, 0x0(r30)
    rlwinm r0, r0, 27, 29, 31
    cmplwi r0, 0x2
    bne @L_80113194
    mr r3, r28
    mr r4, r27
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80113194
    li r4, 0x1
    bl fn_800E90C8
@L_80113194:
    lhz r3, 0x6(r30)
    cmplwi r3, 0x0
    beq @L_8011321C
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801131C4
    mr r3, r28
    mr r4, r27
    li r5, 0x0
    bl fn_80116164
    b @L_8011321C
@L_801131C4:
    mr r3, r28
    mr r4, r27
    li r5, 0x1
    bl fn_80116164
    lhz r3, 0x8(r30)
    cmplwi r3, 0x0
    beq @L_8011321C
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80113200
    mr r3, r28
    mr r4, r27
    li r5, 0x0
    bl fn_8018C1E8
@L_80113200:
    lis r5, 0x596
    lhz r6, 0x6(r30)
    lhz r7, 0x8(r30)
    mr r3, r28
    mr r4, r27
    addi r5, r5, 0xa
    bl fn_801837D8
@L_8011321C:
    addi r31, r31, 0x1c
    addi r29, r29, 0x1
@L_80113224:
    lwz r3, lbl_80478EB8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_80113018
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

