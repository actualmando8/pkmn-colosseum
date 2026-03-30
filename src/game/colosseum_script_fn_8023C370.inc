    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r6
    mr r28, r3
    mr r24, r7
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023C404
    mr r4, r30
    li r3, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_8011BEB4
    mr. r26, r3
    bne @L_8023C40C
    lis r3, -32731
    addi r0, r3, 14664
    mr r26, r0
    b @L_8023C40C
@L_8023C404:
    li r3, 0x0
    b @L_8023C51C
@L_8023C40C:
    mr r4, r28
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r27, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r24, 0, 24, 31
    rlwinm r4, r3, 0, 24, 31
    cmplwi r0, 0x1
    li r24, 0x0
    bne @L_8023C4F8
    cmplwi r4, 0x7
    bgt @L_8023C518
    lis r3, jumptable_8039A598@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8039A598@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r12, r26
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
    mr r24, r3
    b @L_8023C518
    rlwinm r27, r27, 0, 16, 31
    li r25, 0x0
    b @L_8023C4C4
@L_8023C4A0:
    mr r12, r26
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
    add r24, r24, r3
    addi r25, r25, 0x1
@L_8023C4C4:
    rlwinm r0, r25, 0, 16, 31
    cmplw r0, r27
    blt @L_8023C4A0
    b @L_8023C518
    mr r12, r26
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
    mr r24, r3
    b @L_8023C518
@L_8023C4F8:
    mr r12, r26
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
    mr r24, r3
@L_8023C518:
    mr r3, r24
@L_8023C51C:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
