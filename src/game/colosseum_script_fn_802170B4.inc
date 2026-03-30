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
    mr r30, r3
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r29, r3, 0, 24, 31
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021719C
    cmplwi r29, 0x0
    bne @L_8021713C
    mr r3, r31
    li r4, 0x2
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    b @L_80217174
@L_8021713C:
    cmplwi r29, 0x1
    bne @L_80217164
    mr r3, r31
    li r4, 0x1
    bl fn_80203B5C
    rlwinm r3, r3, 0, 16, 31
    li r0, 0x1e
    mulli r3, r3, 0x14
    divw r0, r3, r0
    b @L_80217174
@L_80217164:
    mr r3, r31
    li r4, 0x4
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
@L_80217174:
    mr r3, r30
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_802171A8
@L_8021719C:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802171A8:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
