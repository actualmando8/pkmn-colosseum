    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    lis r3, 0x8000
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    srw r29, r3, r31
    lwz r0, lbl_8047A8A4(r13)
    and. r0, r0, r29
    beq @L_800AAD18
    lwz r5, lbl_8047A8B0(r13)
    nor r6, r29, r29
    lwz r3, lbl_8047A8B4(r13)
    clrlwi. r0, r4, 28
    and r5, r5, r6
    and r3, r3, r6
    stw r5, lbl_8047A8B0(r13)
    stw r3, lbl_8047A8B4(r13)
    clrrwi r3, r4, 8
    bne @L_800AACC0
    clrrwi. r0, r3, 31
    beq @L_800AACC0
    rlwinm. r0, r3, 0, 11, 11
    beq @L_800AACC0
    rlwinm. r0, r3, 0, 1, 1
    beq @L_800AACC0
    rlwinm. r0, r3, 0, 5, 5
    bne @L_800AACC0
    rlwinm. r0, r3, 0, 12, 12
    bne @L_800AACC0
    rlwinm. r0, r3, 0, 13, 13
    bne @L_800AACC0
    mulli r4, r31, 0xc
    lis r3, lbl_803FC5E0@ha
    addi r0, r3, lbl_803FC5E0@l
    lis r3, fn_800AA73C@ha
    add r6, r0, r4
    addi r8, r3, fn_800AA73C@l
    addi r3, r31, 0x0
    la r4, lbl_80478A20(r13)
    li r5, 0x1
    li r7, 0xa
    li r10, 0x0
    li r9, 0x0
    bl SITransfer
    b @L_800AAD18
    @L_800AACC0:
    bl OSDisableInterrupts
    addi r30, r3, 0x0
    addi r3, r29, 0x0
    bl fn_800D0464
    lwz r6, lbl_8047A8A4(r13)
    nor r8, r29, r29
    lwz r5, lbl_8047A8B0(r13)
    mr r3, r31
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
    mr r3, r30
    bl OSRestoreInterrupts
    @L_800AAD18:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
