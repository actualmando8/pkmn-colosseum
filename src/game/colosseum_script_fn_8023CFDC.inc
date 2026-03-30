    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r6
    mr r30, r4
    mr r29, r3
    mr r31, r5
    mr r4, r24
    li r25, 0x0
    bl fn_80236FFC
    mr r26, r3
    mr r3, r29
    mr r4, r24
    bl fn_8023715C
    mr r27, r3
    mr r3, r29
    mr r4, r24
    bl fn_80236520
    mr r28, r3
    mr r3, r29
    mr r4, r30
    mr r5, r31
    mr r6, r24
    bl fn_8023C530
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023D094
    mr r4, r29
    li r3, 0x0
    li r5, 0x21a
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x21a
    bl fn_80239EE8
@L_8023D094:
    rlwinm r3, r27, 0, 16, 31
    rlwinm r0, r26, 0, 16, 31
    cmplw r3, r0
    ble @L_8023D0EC
    mr r3, r25
    mr r4, r29
    li r5, 0x21b
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x21b
    bl fn_80239EE8
@L_8023D0EC:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x118
    bne @L_8023D140
    mr r3, r25
    mr r4, r29
    li r5, 0x21c
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x21c
    bl fn_80239EE8
@L_8023D140:
    mr r3, r25
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
