    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    bl fn_80008174
    rlwinm r0, r3, 0, 24, 31
    mr r30, r3
    cmplwi r0, 0x1
    bne @L_8020BF10
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    li r25, 0x0
    b @L_8020BF00
@L_8020BE94:
    mr r4, r25
    li r3, 0x0
    bl fn_801F47B4
    mr. r29, r3
    beq @L_8020BEFC
    rlwinm r31, r25, 0, 16, 31
    li r26, 0x0
    b @L_8020BEF0
@L_8020BEB4:
    mr r3, r29
    mr r4, r26
    bl fn_801F7258
    cmplwi r3, 0x0
    beq @L_8020BEEC
    mr r4, r27
    mr r5, r30
    bl fn_8026316C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020BEEC
    cmplwi r31, 0x0
    beq @L_8020BEB4
    subi r25, r25, 0x1
    b @L_8020BE94
@L_8020BEEC:
    addi r26, r26, 0x1
@L_8020BEF0:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r28
    blt @L_8020BEB4
@L_8020BEFC:
    addi r25, r25, 0x1
@L_8020BF00:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8020BE94
    b @L_8020BF28
@L_8020BF10:
    lis r4, -32735
    li r3, 0x0
    addi r4, r4, -16576
    li r5, 0x0
    li r6, 0x1
    bl fn_801F2B5C
@L_8020BF28:
    lmw r25, 0x14(r1)
    li r3, 0x1
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
