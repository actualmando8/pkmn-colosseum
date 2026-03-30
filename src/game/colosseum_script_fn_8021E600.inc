    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801F54A4
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    lhz r4, lbl_8047B60C(r13)
    mr r31, r3
    li r3, 0x0
    li r5, 0x9
    mr r30, r4
    li r6, 0x0
    bl fn_8011BEB4
    lwz r4, lbl_8047B610(r13)
    rlwinm r29, r3, 0, 16, 31
    lbz r0, 0x1(r4)
    cmplwi r0, 0x0
    beq @L_8021E680
    mr r3, r31
    mr r4, r30
    bl fn_80209C1C
    b @L_8021E6A4
@L_8021E680:
    mr r3, r31
    rlwinm r7, r30, 0, 16, 31
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    mr r4, r30
    bl fn_80209C1C
@L_8021E6A4:
    lis r3, lbl_80379BFF@ha
    clrlslwi r0, r29, 16, 2
    addi r3, r3, lbl_80379BFF@l
    lwzx r0, r3, r0
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
