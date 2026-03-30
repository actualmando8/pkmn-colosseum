    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r27, 0xc(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r27, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    mr r3, r27
    li r4, 0x0
    li r5, 0x103
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0x102
    li r6, 0x0
    bl fn_8012640C
    cmplwi r29, 0x0
    extsh r30, r3
    beq @L_80219CBC
    extsh. r0, r30
    beq @L_80219CBC
    mr r3, r29
    mr r4, r31
    bl fn_801F00D0
    mr. r29, r3
    beq @L_80219CBC
    mr r3, r27
    mr r4, r29
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80219CBC
    mr r3, r29
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80219CBC
    mr r3, r28
    rlwinm r7, r30, 1, 0, 30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r4, r27
    li r3, 0x3
    bl fn_801F025C
    mr r30, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r30
    li r28, 0x0
    li r4, 0x4d
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80219C88
    mr r3, r30
    li r4, 0x4d
    bl fn_801F6C4C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80219C88
    mr r4, r27
    bl fn_801F00D0
    mr. r27, r3
    beq @L_80219C88
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80219C88
    mr r28, r27
@L_80219C88:
    cmplwi r28, 0x0
    beq @L_80219C94
    mr r29, r28
@L_80219C94:
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80219CE0
@L_80219CBC:
    mr r3, r27
    li r4, 0x0
    li r5, 0x118
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80219CE0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
