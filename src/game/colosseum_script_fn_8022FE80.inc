    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, -32733
    li r3, 0x0
    stw r0, 0x24(r1)
    addi r4, r4, 284
    li r5, 0x0
    li r6, 0x0
    stmw r28, 0x10(r1)
    bl fn_801F37B0
    lwz r28, lbl_8047B62C(r13)
    lis r4, lbl_80378798@ha
    lbz r29, lbl_8047B614(r13)
    li r0, 0x0
    lis r3, lbl_8027A00C@ha
    lwz r30, lbl_8047B610(r13)
    addi r4, r4, lbl_80378798@l
    stb r0, lbl_8047B614(r13)
    addi r31, r3, lbl_8027A00C@l
    stw r4, lbl_8047B610(r13)
    stw r28, lbl_8047B62C(r13)
@L_8022FED4:
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r12, r31, r0
    mtctr r12
    bctrl
    lbz r0, lbl_8047B614(r13)
    cmplwi r0, 0x1
    beq @L_8022FF00
    cmplwi r0, 0x2
    bne @L_8022FED4
@L_8022FF00:
    li r0, 0x1
    lis r3, -32733
    stw r28, lbl_8047B62C(r13)
    addi r4, r3, -7348
    addi r5, r1, 0x9
    li r3, 0x0
    stb r29, lbl_8047B614(r13)
    li r6, 0x0
    stw r30, lbl_8047B610(r13)
    stb r0, 0x9(r1)
    bl fn_801F37B0
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, -7688
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 136
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r0, 0x0
    lis r3, -32733
    stb r0, 0x8(r1)
    addi r4, r3, -5220
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
