    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r26, 0x18(r1)
    mr. r30, r3
    addi r26, r4, 0x0
    addi r27, r5, 0x0
    addi r29, r6, 0x0
    blt @L_800B3768
    cmpwi r30, 0x2
    blt @L_800B3770
    @L_800B3768:
    li r3, -0x80
    b @L_800B38C8
    @L_800B3770:
    lis r3, 0x8000
    lbz r0, 0x30e3(r3)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800B3788
    li r3, -0x3
    b @L_800B38C8
    @L_800B3788:
    mulli r4, r30, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r4
    bl OSDisableInterrupts
    lwz r0, 0x4(r31)
    addi r28, r3, 0x0
    cmpwi r0, -0x1
    bne @L_800B37BC
    mr r3, r28
    bl OSRestoreInterrupts
    li r3, -0x1
    b @L_800B38C8
    @L_800B37BC:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    bne @L_800B37E8
    mr r3, r30
    bl fn_800993A8
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800B37E8
    mr r3, r28
    bl OSRestoreInterrupts
    li r3, -0x2
    b @L_800B38C8
    @L_800B37E8:
    li r0, -0x1
    stw r0, 0x4(r31)
    cmplwi r29, 0x0
    stw r26, 0x80(r31)
    stw r27, 0xc4(r31)
    beq @L_800B3808
    mr r0, r29
    b @L_800B3810
    @L_800B3808:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B3810:
    stw r0, 0xd0(r31)
    li r0, 0x0
    stw r0, 0xcc(r31)
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    bne @L_800B3858
    lis r3, fn_800AF284@ha
    addi r4, r3, fn_800AF284@l
    addi r3, r30, 0x0
    bl fn_800989C0
    cmpwi r3, 0x0
    bne @L_800B3858
    li r0, -0x3
    stw r0, 0x4(r31)
    mr r3, r28
    bl OSRestoreInterrupts
    li r3, -0x3
    b @L_800B38C8
    @L_800B3858:
    li r29, 0x0
    stw r29, 0x24(r31)
    li r0, 0x1
    addi r3, r30, 0x0
    stw r0, 0x0(r31)
    li r4, 0x0
    bl fn_8009870C
    addi r3, r31, 0xe0
    bl fn_8009A590
    stw r29, 0x84(r31)
    mr r3, r28
    stw r29, 0x88(r31)
    bl OSRestoreInterrupts
    lis r3, fn_800B3604@ha
    addi r0, r3, fn_800B3604@l
    lis r3, fn_800AF51C@ha
    stw r0, 0xdc(r31)
    addi r5, r3, fn_800AF51C@l
    addi r3, r30, 0x0
    li r4, 0x0
    bl EXILock
    cmpwi r3, 0x0
    bne @L_800B38BC
    li r3, 0x0
    b @L_800B38C8
    @L_800B38BC:
    stw r29, 0xdc(r31)
    mr r3, r30
    bl fn_800B31F4
    @L_800B38C8:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
