    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    li r4, 0x0
    li r5, 0xf8
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r31, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r29
    mr r28, r0
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80217EAC
    mr r3, r30
    bl fn_80200B10
    lis r3, lbl_80375FDF@ha
    addi r0, r3, lbl_80375FDF@l
    stw r0, lbl_8047B610(r13)
    b @L_80218004
@L_80217EAC:
    mr r3, r30
    li r4, 0x2f
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80217F04
    mr r3, r30
    li r4, 0x2f
    li r5, 0x0
    bl fn_8020248C
    mr r3, r30
    li r4, 0x22
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80217F04
    mr r3, r30
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    mr r4, r29
    bl fn_8020A2B8
@L_80217F04:
    mr r3, r30
    li r27, 0x1
    li r4, 0x2f
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217F60
    li r3, 0x2f
    bl fn_80119DD0
    rlwinm r31, r3, 0, 24, 31
    mr r3, r30
    li r4, 0x2f
    bl fn_80202360
    mr r27, r3
    extsh r0, r27
    cmpw r0, r31
    bne @L_80217F60
    mr r3, r30
    li r4, 0x2f
    bl fn_80202810
    mr r3, r30
    li r4, 0x22
    bl fn_80202810
@L_80217F60:
    mr r4, r28
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r4, r3, 0, 16, 31
    extsh r3, r27
    li r5, 0x1
    b @L_80217F8C
@L_80217F84:
    clrlslwi r4, r4, 17, 1
    addi r5, r5, 0x1
@L_80217F8C:
    rlwinm r0, r5, 0, 16, 31
    cmpw r0, r3
    blt @L_80217F84
    mr r3, r29
    rlwinm r7, r4, 0, 16, 31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r30
    li r4, 0x1a
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217FF8
    mr r3, r29
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    mr r3, r29
    clrlslwi r7, r0, 17, 1
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
@L_80217FF8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80218004:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
