    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2f0(r1)
    stw r31, 0x2ec(r1)
    addi r31, r3, 0x0
    lis r3, 0x8048
    stw r30, 0x2e8(r1)
    slwi r5, r31, 8
    stw r29, 0x2e4(r1)
    lwz r0, -20912(r13)
    cmpwi r0, 0x0
    addi r0, r3, -31776
    add r30, r0, r5
    bne @L_8025F990
    clrlwi. r0, r4, 24
    bne @L_8025F8D8
    clrrwi r3, r4, 16
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_8025F8E4
@L_8025F8D8:
    li r0, 0x1
    stw r0, 0x20(r30)
    b @L_8025F91C
@L_8025F8E4:
    lis r3, 0x8026
    lwz r5, 0xc(r30)
    addi r8, r3, -2292
    lwz r7, 0x10(r30)
    lwz r9, 0x30(r30)
    mr r3, r31
    lwz r10, 0x34(r30)
    addi r4, r30, 0x0
    addi r6, r30, 0x5
    bl SITransfer
    cmpwi r3, 0x0
    bne @L_8025F990
    li r0, 0x2
    stw r0, 0x20(r30)
@L_8025F91C:
    lwz r12, 0x38(r30)
    cmplwi r12, 0x0
    beq @L_8025F93C
    li r0, 0x0
    mtlr r12
    stw r0, 0x38(r30)
    mr r3, r31
    blrl
@L_8025F93C:
    lwz r0, 0x1c(r30)
    cmplwi r0, 0x0
    beq @L_8025F990
    bl fn_8009BBC4
    addi r29, r3, 0x0
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, 0x1c(r30)
    li r0, 0x0
    addi r3, r31, 0x0
    stw r0, 0x1c(r30)
    mtlr r12
    lwz r4, 0x20(r30)
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r29
    bl OSSetCurrentContext
    bl __OSReschedule
@L_8025F990:
    lwz r0, 0x2f4(r1)
    lwz r31, 0x2ec(r1)
    lwz r30, 0x2e8(r1)
    lwz r29, 0x2e4(r1)
    addi r1, r1, 0x2f0
    mtlr r0
    blr
