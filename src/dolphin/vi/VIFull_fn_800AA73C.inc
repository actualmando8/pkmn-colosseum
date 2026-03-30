    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    lis r0, 0x8000
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r4, 0x0
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    mr r29, r3
    srw r30, r0, r29
    lwz r3, lbl_8047A8A4(r13)
    and. r0, r3, r30
    beq @L_800AA7E0
    clrlwi. r0, r31, 28
    bne @L_800AA780
    mr r3, r29
    bl fn_800AA4D4
    @L_800AA780:
    rlwinm. r0, r31, 0, 28, 28
    beq @L_800AA7E0
    bl OSDisableInterrupts
    addi r31, r3, 0x0
    addi r3, r30, 0x0
    bl fn_800D0464
    lwz r6, lbl_8047A8A4(r13)
    nor r8, r30, r30
    lwz r5, lbl_8047A8B0(r13)
    mr r3, r29
    lwz r4, lbl_8047A8B4(r13)
    lwz r0, lbl_8047A8B8(r13)
    and r7, r6, r8
    and r6, r5, r8
    stw r7, lbl_8047A8A4(r13)
    and r5, r4, r8
    and r0, r0, r8
    stw r6, lbl_8047A8B0(r13)
    li r4, 0x0
    stw r5, lbl_8047A8B4(r13)
    stw r0, lbl_8047A8B8(r13)
    bl fn_800A115C
    mr r3, r31
    bl OSRestoreInterrupts
    @L_800AA7E0:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
