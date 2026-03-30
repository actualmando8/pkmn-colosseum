    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80217C90
    mr r3, r31
    li r4, 0x2e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217C80
    mr r3, r31
    li r4, 0x2e
    bl fn_80202810
@L_80217C80:
    lis r3, lbl_80375FDF@ha
    addi r0, r3, lbl_80375FDF@l
    stw r0, lbl_8047B610(r13)
    b @L_80217D20
@L_80217C90:
    mr r3, r31
    li r4, 0x2e
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80217CB8
    mr r3, r31
    li r4, 0x2e
    li r5, 0x0
    bl fn_8020248C
@L_80217CB8:
    mr r3, r31
    li r4, 0x2e
    bl fn_80202360
    mr r31, r3
    mr r4, r29
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    extsh r4, r31
    rlwinm r3, r3, 0, 16, 31
    subi r0, r4, 0x1
    mr r7, r3
    mtctr r0
    cmpwi r4, 0x1
    ble @L_80217D00
@L_80217CF8:
    rlwinm r7, r7, 1, 0, 30
    bdnz @L_80217CF8
@L_80217D00:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80217D20:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
