    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0xa8(r1)
    stw r31, 0xa4(r1)
    stw r30, 0xa0(r1)
    stw r29, 0x9c(r1)
    bl fn_800B7714
    mr. r31, r3
    beq @L_800B76F0
    bl fn_800B770C
    li r0, 0x10
    lwz r29, 0x0(r31)
    mtctr r0
    addi r30, r3, 0x0
    addi r5, r1, 0xc
    subi r4, r31, 0x8
@L_800B764C:
    lwzu r3, 0x8(r4)
    lwz r0, 0x4(r4)
    stwu r3, 0x8(r5)
    stw r0, 0x4(r5)
    bdnz @L_800B764C
    bl OSDisableInterrupts
    stw r29, 0x28(r1)
    li r0, 0x0
    stw r29, 0x2c(r1)
    stw r0, 0x30(r1)
    lwz r4, 0x30(r1)
    cmpwi r4, 0x0
    bge @L_800B768C
    lwz r0, 0x1c(r1)
    add r0, r4, r0
    stw r0, 0x30(r1)
@L_800B768C:
    bl OSRestoreInterrupts
    addi r3, r1, 0x14
    bl GXSetGPFifo
    cmplw r30, r31
    bne @L_800B76A8
    addi r3, r1, 0x14
    bl GXSetCPUFifo
@L_800B76A8:
    bl OSDisableInterrupts
    stw r29, 0x14(r31)
    li r0, 0x0
    stw r29, 0x18(r31)
    stw r0, 0x1c(r31)
    lwz r4, 0x1c(r31)
    cmpwi r4, 0x0
    bge @L_800B76D4
    lwz r0, 0x8(r31)
    add r0, r4, r0
    stw r0, 0x1c(r31)
@L_800B76D4:
    bl OSRestoreInterrupts
    mr r3, r31
    bl GXSetGPFifo
    cmplw r30, r31
    bne @L_800B76F0
    mr r3, r30
    bl GXSetCPUFifo
@L_800B76F0:
    lwz r0, 0xac(r1)
    lwz r31, 0xa4(r1)
    lwz r30, 0xa0(r1)
    lwz r29, 0x9c(r1)
    addi r1, r1, 0xa8
    mtlr r0
    blr
