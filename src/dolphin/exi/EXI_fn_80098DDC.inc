    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2f8(r1)
    stmw r27, 0x2e4(r1)
    sth r3, 0x8(r1)
    mr r27, r4
    lha r3, 0x8(r1)
    subi r3, r3, 0x9
    li r0, 0x3
    divw r31, r3, r0
    rlwinm r4, r31, 6, 0, 25
    lis r3, -32704
    addi r0, r3, -19512
    add r28, r0, r4
    mulli r0, r31, 0x5
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    lwzx r30, r3, r0
    stw r30, 0x10(r1)
    andi. r30, r30, 0x7f5
    ori r30, r30, 0x2
    mulli r0, r31, 0x5
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    stwx r30, r3, r0
    lwz r29, 0x0(r28)
    cmplwi r29, 0x0
    beq @L_80098E88
    addi r3, r1, 0x18
    bl OSClearContext
    addi r3, r1, 0x18
    bl OSSetCurrentContext
    addi r3, r31, 0x0
    addi r4, r27, 0x0
    addi r12, r29, 0x0
    mtlr r12
    blrl
    addi r3, r1, 0x18
    bl OSClearContext
    mr r3, r27
    bl OSSetCurrentContext
@L_80098E88:
    lwz r0, 0x2fc(r1)
    lmw r27, 0x2e4(r1)
    addi r1, r1, 0x2f8
    mtlr r0
    blr
