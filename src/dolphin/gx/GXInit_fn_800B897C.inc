    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    lwz r3, -29632(r13)
    lbz r0, 0x4f2(r3)
    cmplwi r0, 0x0
    beq @L_800B8A48
    bl fn_800B7714
    cmplwi r3, 0x0
    beq @L_800B8A48
    lwz r3, -24220(r13)
    addi r6, r3, 0x4e
    lhz r4, 0x4e(r3)
    addi r5, r3, 0x50
@L_800B89B8:
    mr r0, r4
    lhz r4, 0x0(r6)
    lhz r3, 0x0(r5)
    cmplw r4, r0
    bne @L_800B89B8
    rlwinm r0, r4, 16, 0, 15
    or r27, r0, r3
@L_800B89D4:
    bl OSGetTime
    li r0, 0x0
    addi r31, r4, 0x0
    addi r30, r3, 0x0
    xoris r28, r0, 0x8000
    li r29, 0x8
@L_800B89EC:
    bl OSGetTime
    subfc r4, r31, r4
    subfe r0, r30, r3
    xoris r3, r0, 0x8000
    subfc r0, r4, r29
    subfe r3, r3, r28
    subfe r3, r28, r28
    neg. r3, r3
    beq @L_800B89EC
    lwz r3, -24220(r13)
    addi r6, r3, 0x4e
    lhz r4, 0x4e(r3)
    addi r5, r3, 0x50
@L_800B8A20:
    mr r0, r4
    lhz r4, 0x0(r6)
    lhz r3, 0x0(r5)
    cmplw r4, r0
    bne @L_800B8A20
    rlwinm r0, r4, 16, 0, 15
    or r0, r0, r3
    cmplw r0, r27
    mr r27, r0
    bne @L_800B89D4
@L_800B8A48:
    lis r3, 0xcc00
    li r0, 0x1
    addi r27, r3, 0x3000
    stwu r0, 0x18(r27)
    bl OSGetTime
    li r0, 0x0
    addi r31, r4, 0x0
    addi r30, r3, 0x0
    xoris r28, r0, 0x8000
    li r29, 0x32
@L_800B8A70:
    bl OSGetTime
    subfc r4, r31, r4
    subfe r0, r30, r3
    xoris r3, r0, 0x8000
    subfc r0, r4, r29
    subfe r3, r3, r28
    subfe r3, r28, r28
    neg. r3, r3
    beq @L_800B8A70
    li r30, 0x0
    stw r30, 0x0(r27)
    bl OSGetTime
    addi r28, r4, 0x0
    addi r29, r3, 0x0
    xoris r31, r30, 0x8000
    li r30, 0x5
@L_800B8AB0:
    bl OSGetTime
    subfc r4, r28, r4
    subfe r0, r29, r3
    xoris r3, r0, 0x8000
    subfc r0, r4, r30
    subfe r3, r3, r31
    subfe r3, r31, r31
    neg. r3, r3
    beq @L_800B8AB0
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
