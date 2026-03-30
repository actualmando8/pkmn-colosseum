    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x2
    bl fn_80203B5C
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xe6
    li r6, 0x0
    bl fn_8012640C
    extsb r0, r3
    cmpwi r0, 0xc
    bge @L_80217504
    rlwinm r0, r31, 0, 16, 31
    cmplw r28, r0
    ble @L_80217504
    mr r3, r30
    li r4, 0x0
    li r5, 0xe6
    li r6, 0x0
    li r7, 0xc
    bl fn_801254B4
    mr r3, r29
    rlwinm r7, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80217510
@L_80217504:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80217510:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
