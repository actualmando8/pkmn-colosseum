    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r4, 0x0
    stw r28, 0x10(r1)
    addi r28, r3, 0x0
    mulli r5, r28, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    bl OSDisableInterrupts
    lwz r0, 0x0(r31)
    addi r30, r3, 0x0
    cmpwi r0, 0x0
    beq @L_800B3950
    addi r3, r28, 0x0
    li r4, 0x0
    bl fn_8009870C
    mr r3, r28
    bl fn_80098AE8
    addi r3, r31, 0xe0
    bl fn_8009A590
    li r0, 0x0
    stw r0, 0x0(r31)
    stw r29, 0x4(r31)
    stw r0, 0x24(r31)
    @L_800B3950:
    mr r3, r30
    bl OSRestoreInterrupts
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
