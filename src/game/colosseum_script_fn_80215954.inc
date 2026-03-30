    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r26, 0x8(r1)
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x102
    li r6, 0x0
    bl fn_8012640C
    extsh r28, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x103
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    bl fn_8012640C
    extsh r27, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x105
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r4, r26
    bl fn_801F0134
    extsh. r0, r28
    beq @L_80215A2C
    rlwinm r0, r3, 0, 16, 31
    cmplw r29, r0
    beq @L_80215A40
@L_80215A2C:
    extsh. r0, r27
    beq @L_80215A58
    rlwinm r0, r3, 0, 16, 31
    cmplw r31, r0
    bne @L_80215A58
@L_80215A40:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
@L_80215A58:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
