    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x310(r1)
    stmw r21, 0x2e4(r1)
    addi r21, r3, 0x0
    addi r22, r4, 0x0
    extsh r3, r21
    subi r3, r3, 0xa
    li r0, 0x3
    divw r31, r3, r0
    rlwinm r4, r31, 6, 0, 25
    lis r3, -32704
    addi r0, r3, -19512
    add r27, r0, r4
    lis r3, 0x8000
    extsh r0, r21
    srw r3, r3, r0
    bl __OSMaskInterrupts
    mulli r0, r31, 0x5
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    lwzx r29, r3, r0
    stw r29, 0x10(r1)
    andi. r29, r29, 0x7f5
    ori r29, r29, 0x8
    mulli r0, r31, 0x5
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    stwx r29, r3, r0
    lwz r26, 0x4(r27)
    cmplwi r26, 0x0
    beq @L_80098FE4
    li r0, 0x0
    stw r0, 0x4(r27)
    rlwinm r4, r31, 6, 0, 25
    lis r3, -32704
    addi r0, r3, -19512
    add r30, r0, r4
    lwz r0, 0xc(r30)
    rlwinm. r0, r0, 0, 30, 31
    beq @L_80098FB0
    lwz r0, 0xc(r30)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_80098FA4
    lwz r25, 0x10(r30)
    cmpwi r25, 0x0
    beq @L_80098FA4
    lwz r23, 0x14(r30)
    mulli r3, r31, 0x5
    addi r0, r3, 0x4
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    lwzx r24, r3, r0
    li r28, 0x0
    b @L_80098F9C
@L_80098F84:
    subfic r0, r28, 0x3
    rlwinm r0, r0, 3, 0, 28
    srw r3, r24, r0
    stb r3, 0x0(r23)
    addi r23, r23, 0x1
    addi r28, r28, 0x1
@L_80098F9C:
    cmpw r28, r25
    blt @L_80098F84
@L_80098FA4:
    lwz r0, 0xc(r30)
    rlwinm r0, r0, 0, 0, 29
    stw r0, 0xc(r30)
@L_80098FB0:
    addi r3, r1, 0x18
    bl OSClearContext
    addi r3, r1, 0x18
    bl OSSetCurrentContext
    addi r3, r31, 0x0
    addi r4, r22, 0x0
    addi r12, r26, 0x0
    mtlr r12
    blrl
    addi r3, r1, 0x18
    bl OSClearContext
    mr r3, r22
    bl OSSetCurrentContext
@L_80098FE4:
    lwz r0, 0x314(r1)
    lmw r21, 0x2e4(r1)
    addi r1, r1, 0x310
    mtlr r0
    blr
