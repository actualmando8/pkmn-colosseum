    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2f8(r1)
    stw r31, 0x2f4(r1)
    stw r30, 0x2f0(r1)
    addi r30, r5, 0x0
    stw r29, 0x2ec(r1)
    addi r29, r3, 0x0
    lis r3, 0x8048
    lwz r0, -20912(r13)
    slwi r6, r29, 8
    cmpwi r0, 0x0
    addi r0, r3, -31776
    add r31, r0, r6
    bne @L_8025F7CC
    clrlwi. r0, r4, 28
    beq @L_8025F75C
    li r0, 0x1
    stw r0, 0x20(r31)
    b @L_8025F764
@L_8025F75C:
    li r0, 0x0
    stw r0, 0x20(r31)
@L_8025F764:
    lwz r12, 0x38(r31)
    cmplwi r12, 0x0
    beq @L_8025F784
    li r0, 0x0
    mtlr r12
    stw r0, 0x38(r31)
    mr r3, r29
    blrl
@L_8025F784:
    lwz r0, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_8025F7CC
    addi r3, r1, 0x18
    bl OSClearContext
    addi r3, r1, 0x18
    bl OSSetCurrentContext
    lwz r12, 0x1c(r31)
    li r0, 0x0
    addi r3, r29, 0x0
    stw r0, 0x1c(r31)
    mtlr r12
    lwz r4, 0x20(r31)
    blrl
    addi r3, r1, 0x18
    bl OSClearContext
    mr r3, r30
    bl OSSetCurrentContext
@L_8025F7CC:
    lwz r0, 0x2fc(r1)
    lwz r31, 0x2f4(r1)
    lwz r30, 0x2f0(r1)
    lwz r29, 0x2ec(r1)
    addi r1, r1, 0x2f8
    mtlr r0
    blr
