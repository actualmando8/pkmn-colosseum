    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x34(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r25, 0x14(r1)
    li r28, 0x0
    li r27, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    mr r4, r31
    bl fn_801F4354
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x2e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212DF0
    mr r3, r31
    li r4, 0x2e
    bl fn_80202810
@L_80212DF0:
    mr r3, r31
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212E14
    mr r3, r31
    li r4, 0x15
    bl fn_80202810
@L_80212E14:
    mr r3, r31
    li r4, 0x28
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212E38
    mr r3, r31
    li r4, 0x28
    bl fn_80202810
@L_80212E38:
    mr r3, r31
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F38
    mr r3, r31
    bl fn_80205B8C
    bl fn_8011F634
    mr r25, r3
    mr r3, r31
    li r4, 0x3e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F0C
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r3, r5, r4
    rlwinm r0, r25, 0, 24, 31
    mullw r3, r3, r4
    subf r3, r3, r5
    cmpw r3, r0
    bge @L_80212EC4
    mr r3, r31
    li r29, 0x7707
    li r27, 0x1
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F60
    li r28, 0x1
    b @L_80212F60
@L_80212EC4:
    mr r3, r31
    li r29, 0x7708
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F60
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r3, r5, r4
    rlwinm r0, r25, 31, 25, 31
    mullw r3, r3, r4
    subf r3, r3, r5
    cmpw r3, r0
    bge @L_80212F60
    li r28, 0x1
    b @L_80212F60
@L_80212F0C:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F30
    li r29, 0x7706
    li r28, 0x1
    b @L_80212F60
@L_80212F30:
    li r29, 0x771d
    b @L_80212F60
@L_80212F38:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212F5C
    li r29, 0x7706
    li r28, 0x1
    b @L_80212F60
@L_80212F5C:
    li r29, 0x7709
@L_80212F60:
    mr r3, r30
    li r4, 0x0
    bl fn_801FB974
    mr r3, r31
    mr r4, r28
    mr r5, r27
    li r6, 0x0
    li r7, 0x0
    bl fn_802080A8
    mr r3, r30
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    li r3, 0x1
    bl fn_801EF8F4
    li r3, 0x7704
    bl fn_802624CC
    li r3, 0x40
    bl fn_801F000C
    bl fn_8026246C
    mr r3, r30
    li r4, 0x1
    bl fn_801FB974
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
    li r3, 0x7705
    bl fn_802624CC
    mr r3, r30
    li r4, 0x2
    bl fn_801FB974
    bl fn_8026246C
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213034
    mr r3, r31
    li r4, 0x8
    bl fn_80202810
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213034
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80213034:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802130C4
    mr r3, r31
    li r4, 0x3e
    bl fn_80202810
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213074
    mr r3, r31
    li r4, 0xc8
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80213074:
    mr r3, r31
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802130C4
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x2
    bl fn_8011F910
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802130C4
    mr r3, r31
    li r4, 0xc5
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_802130C4:
    mr r3, r31
    mr r4, r28
    mr r5, r27
    mr r6, r29
    li r7, 0x1
    bl fn_802080A8
    mr r3, r31
    mr r4, r28
    mr r5, r27
    li r6, 0x0
    li r7, 0x2
    bl fn_802080A8
    bl fn_8026246C
    mr r3, r31
    mr r4, r26
    li r5, 0x0
    bl fn_8026532C
    mr r3, r30
    li r4, 0x3
    bl fn_801FB974
    mr r3, r31
    mr r4, r28
    mr r5, r27
    li r6, 0x0
    li r7, 0x3
    bl fn_802080A8
    bl fn_801DA7AC
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
