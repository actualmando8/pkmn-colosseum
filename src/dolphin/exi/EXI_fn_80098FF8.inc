    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2e8(r1)
    stmw r28, 0x2d8(r1)
    sth r3, 0x8(r1)
    mr r28, r4
    lha r3, 0x8(r1)
    subi r3, r3, 0xb
    li r0, 0x3
    divw r31, r3, r0
    lis r3, 0x70
    mulli r0, r31, 0x3
    srw r3, r3, r0
    bl __OSMaskInterrupts
    li r4, 0x0
    mulli r0, r31, 0x5
    rlwinm r0, r0, 2, 0, 29
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    stwx r4, r3, r0
    rlwinm r4, r31, 6, 0, 25
    lis r3, -32704
    addi r0, r3, -19512
    add r30, r0, r4
    lwz r29, 0x8(r30)
    lwz r0, 0xc(r30)
    rlwinm r0, r0, 0, 29, 27
    stw r0, 0xc(r30)
    cmplwi r29, 0x0
    beq @L_800990AC
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    li r0, 0x0
    stw r0, 0x8(r30)
    addi r3, r31, 0x0
    addi r4, r28, 0x0
    addi r12, r29, 0x0
    mtlr r12
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r28
    bl OSSetCurrentContext
@L_800990AC:
    lwz r0, 0x2ec(r1)
    lmw r28, 0x2d8(r1)
    addi r1, r1, 0x2e8
    mtlr r0
    blr
