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
    mr r28, r0
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x14
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80219688
    add r0, r30, r31
    mr r3, r28
    srawi r0, r0, 1
    li r4, 0x0
    addze r7, r0
    li r5, 0x2d
    subf r0, r7, r31
    li r6, 0x0
    stw r0, lbl_8047B608(r13)
    subf r7, r7, r30
    bl fn_8011BBD8
    lis r4, 0x1
    mr r3, r29
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80219694
@L_80219688:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80219694:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
