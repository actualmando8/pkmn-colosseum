    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r29, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0x99
    li r6, 0x0
    bl fn_8012640C
    cmplwi r30, 0x79
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bne @L_80217B94
    mulli r7, r0, 0xa
    li r0, 0x19
    divw r0, r7, r0
    rlwinm r0, r0, 0, 16, 31
    b @L_80217BA8
@L_80217B94:
    subfic r7, r0, 0xff
    li r0, 0x19
    mulli r7, r7, 0xa
    divw r0, r7, r0
    rlwinm r0, r0, 0, 16, 31
@L_80217BA8:
    rlwinm r7, r0, 0, 16, 31
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
