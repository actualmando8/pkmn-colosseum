    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r27, 0x1c(r1)
    addi r27, r3, 0x0
    addi r28, r4, 0x0
    addi r29, r5, 0x0
    bl OSDisableInterrupts
    mulli r5, r27, 0x110
    lis r4, lbl_803FC620@ha
    addi r0, r4, lbl_803FC620@l
    add r31, r0, r5
    lwz r0, 0x0(r31)
    addi r30, r3, 0x0
    cmpwi r0, 0x0
    bne @L_800AFC24
    li r31, -0x3
    b @L_800AFD70
    @L_800AFC24:
    cmplwi r28, 0x0
    beq @L_800AFC30
    stw r28, 0xc8(r31)
    @L_800AFC30:
    cmplwi r29, 0x0
    beq @L_800AFC3C
    stw r29, 0xcc(r31)
    @L_800AFC3C:
    lis r3, fn_800AFACC@ha
    addi r0, r3, fn_800AFACC@l
    lis r3, fn_800AF51C@ha
    stw r0, 0xdc(r31)
    addi r5, r3, fn_800AF51C@l
    addi r3, r27, 0x0
    li r4, 0x0
    bl EXILock
    cmpwi r3, 0x0
    bne @L_800AFC6C
    li r31, -0x1
    b @L_800AFD70
    @L_800AFC6C:
    li r0, 0x0
    stw r0, 0xdc(r31)
    addi r3, r27, 0x0
    li r4, 0x0
    li r5, 0x4
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800AFC9C
    mr r3, r27
    bl EXIUnlock
    li r31, -0x3
    b @L_800AFD70
    @L_800AFC9C:
    addi r3, r31, 0xe0
    bl fn_8009A590
    lbz r0, 0x94(r31)
    cmpwi r0, 0xf3
    beq @L_800AFD6C
    bge @L_800AFCC4
    cmpwi r0, 0xf1
    beq @L_800AFD08
    bge @L_800AFCD0
    b @L_800AFD6C
    @L_800AFCC4:
    cmpwi r0, 0xf5
    bge @L_800AFD6C
    b @L_800AFD08
    @L_800AFCD0:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    lis r4, 0x1062
    lis r3, fn_800AF7FC@ha
    srwi r0, r0, 2
    addi r4, r4, 0x4dd3
    mulhwu r0, r4, r0
    srwi r0, r0, 6
    mulli r6, r0, 0x64
    addi r7, r3, fn_800AF7FC@l
    addi r3, r31, 0xe0
    li r5, 0x0
    bl OSSetAlarm
    b @L_800AFD6C
    @L_800AFD08:
    lis r3, 0x8000
    lwz r4, 0xc(r31)
    lwz r0, 0xf8(r3)
    lis r3, fn_800AF7FC@ha
    srawi r9, r4, 13
    srwi r7, r0, 2
    li r0, 0x2
    li r4, 0x0
    mullw r8, r4, r0
    mulhwu r6, r7, r0
    add r8, r8, r6
    mullw r5, r7, r0
    addze r9, r9
    mullw r6, r7, r4
    srawi r0, r9, 31
    mullw r4, r0, r5
    mulhwu r0, r9, r5
    addi r7, r3, fn_800AF7FC@l
    add r3, r8, r6
    add r4, r4, r0
    mullw r0, r9, r3
    mullw r6, r9, r5
    addi r3, r31, 0xe0
    add r5, r4, r0
    bl OSSetAlarm
    @L_800AFD6C:
    li r31, 0x0
    @L_800AFD70:
    mr r3, r30
    bl OSRestoreInterrupts
    mr r3, r31
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
