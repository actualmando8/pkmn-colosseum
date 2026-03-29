    nofralloc
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x4(r1)
    lis r8, RunQueue_803FB898@ha
    addi r10, r4, 0x0
    stwu r1, -0x20(r1)
    addi r9, r5, 0x0
    addi r7, r6, 0x0
    stw r31, 0x1c(r1)
    addi r31, r8, RunQueue_803FB898@l
    beq @L_800A26A0
    lhz r0, 0x3c8(r31)
    cmplwi r0, 0x0
    bne @L_800A26B4
    addi r4, r3, 0x0
    addi r6, r9, 0x0
    addi r5, r10, 0x0
    addi r3, r31, 0x100
    li r8, 0x1f
    li r9, 0x1
    bl fn_800A19CC
    addi r3, r31, 0x100
    bl fn_800A1F94
    addi r3, r31, 0x100
    b @L_800A26B8
    @L_800A26A0:
    lhz r0, 0x3c8(r31)
    cmplwi r0, 0x0
    beq @L_800A26B4
    addi r3, r31, 0x100
    bl OSCancelThread
    @L_800A26B4:
    li r3, 0x0
    @L_800A26B8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
