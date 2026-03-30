    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x50(r1)
    stmw r20, 0x20(r1)
    mr r31, r3
    bl OSDisableInterrupts
    li r21, 0x0
    mulli r0, r21, 0xc
    lis r4, lbl_803FC5E0@ha
    addi r26, r4, lbl_803FC5E0@l
    lis r4, fn_800AA73C@ha
    lis r5, fn_800AAC00@ha
    lis r6, fn_800AA8D4@ha
    add r24, r26, r0
    addi r22, r3, 0x0
    addi r30, r4, fn_800AA73C@l
    addi r29, r5, fn_800AAC00@l
    addi r28, r6, fn_800AA8D4@l
    li r20, 0x0
    lis r27, 0x8000
    @L_800AB1A0:
    lwz r0, lbl_8047A8B8(r13)
    srw r23, r27, r21
    and. r0, r0, r23
    beq @L_800AB284
    bl OSDisableInterrupts
    lwz r4, lbl_8047A8B0(r13)
    li r6, 0x0
    lwz r0, lbl_8047A8B4(r13)
    mr r25, r3
    lwz r5, lbl_8047A8B8(r13)
    or r4, r4, r0
    lwz r0, lbl_8047A8A8(r13)
    andc r7, r5, r4
    lwz r5, lbl_8047A8A4(r13)
    or r4, r0, r7
    lwz r0, lbl_80478A18(r13)
    stw r4, lbl_8047A8A8(r13)
    andc r4, r5, r7
    cmplwi r0, 0x4
    lwz r0, lbl_8047A8A8(r13)
    stw r6, lbl_8047A8B8(r13)
    and r3, r0, r5
    stw r4, lbl_8047A8A4(r13)
    bne @L_800AB20C
    lwz r0, lbl_8047A8AC(r13)
    or r0, r0, r7
    stw r0, lbl_8047A8AC(r13)
    @L_800AB20C:
    bl fn_800D0464
    lwz r0, lbl_80478A0C(r13)
    cmpwi r0, 0x20
    bne @L_800AB260
    lwz r4, lbl_8047A8A8(r13)
    cntlzw r0, r4
    stw r0, lbl_80478A0C(r13)
    lwz r3, lbl_80478A0C(r13)
    cmpwi r3, 0x20
    beq @L_800AB260
    mulli r0, r3, 0xc
    srw r3, r27, r3
    andc r4, r4, r3
    stw r4, lbl_8047A8A8(r13)
    add r3, r26, r0
    li r4, 0x0
    li r5, 0xc
    bl memset
    lwz r3, lbl_80478A0C(r13)
    mr r4, r28
    bl fn_800D0CBC
    @L_800AB260:
    mr r3, r25
    bl OSRestoreInterrupts
    li r0, -0x2
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB284:
    lwz r0, lbl_8047A8A8(r13)
    and. r0, r0, r23
    bne @L_800AB29C
    lwz r0, lbl_80478A0C(r13)
    cmpw r0, r21
    bne @L_800AB2B8
    @L_800AB29C:
    li r0, -0x2
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB2B8:
    lwz r0, lbl_8047A8A4(r13)
    and. r0, r0, r23
    bne @L_800AB2E0
    li r0, -0x1
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB2E0:
    mr r3, r21
    bl fn_800CF728
    cmpwi r3, 0x0
    beq @L_800AB30C
    li r0, -0x3
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB30C:
    mr r3, r21
    bl fn_800D02BC
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800AB3E4
    addi r3, r21, 0x0
    addi r4, r1, 0x14
    bl fn_800D05A4
    lwz r0, lbl_8047A8B0(r13)
    and. r0, r0, r23
    beq @L_800AB370
    li r0, 0x0
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    lwz r3, lbl_8047A8B4(r13)
    and. r0, r3, r23
    bne @L_800AB4C8
    or r0, r3, r23
    stw r0, lbl_8047A8B4(r13)
    addi r3, r21, 0x0
    addi r4, r29, 0x0
    bl fn_800D0CBC
    b @L_800AB4C8
    @L_800AB370:
    bl OSDisableInterrupts
    addi r25, r3, 0x0
    addi r3, r23, 0x0
    bl fn_800D0464
    lwz r6, lbl_8047A8A4(r13)
    nor r8, r23, r23
    lwz r5, lbl_8047A8B0(r13)
    mr r3, r21
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
    mr r3, r25
    bl OSRestoreInterrupts
    li r0, -0x1
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB3E4:
    mr r3, r21
    bl SIGetType
    rlwinm. r0, r3, 0, 2, 2
    bne @L_800AB3F8
    or r20, r20, r23
    @L_800AB3F8:
    addi r3, r21, 0x0
    addi r4, r1, 0x14
    bl fn_800D05A4
    cmpwi r3, 0x0
    bne @L_800AB428
    li r0, -0x3
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB428:
    lwz r0, 0x14(r1)
    clrrwi. r0, r0, 31
    beq @L_800AB450
    li r0, -0x3
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    b @L_800AB4C8
    @L_800AB450:
    lwz r12, lbl_80478A1C(r13)
    addi r3, r21, 0x0
    addi r4, r31, 0x0
    mtlr r12
    addi r5, r1, 0x14
    blrl
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 18, 18
    beq @L_800AB4B4
    li r0, -0x3
    stb r0, 0xa(r31)
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0xa
    bl memset
    addi r3, r21, 0x0
    addi r6, r24, 0x0
    addi r8, r30, 0x0
    la r4, lbl_80478A20(r13)
    li r5, 0x1
    li r7, 0xa
    li r10, 0x0
    li r9, 0x0
    bl SITransfer
    b @L_800AB4C8
    @L_800AB4B4:
    li r0, 0x0
    stb r0, 0xa(r31)
    lhz r0, 0x0(r31)
    rlwinm r0, r0, 0, 25, 23
    sth r0, 0x0(r31)
    @L_800AB4C8:
    addi r21, r21, 0x1
    cmpwi r21, 0x4
    addi r24, r24, 0xc
    addi r31, r31, 0xc
    blt @L_800AB1A0
    mr r3, r22
    bl OSRestoreInterrupts
    mr r3, r20
    lmw r20, 0x20(r1)
    lwz r0, 0x54(r1)
    addi r1, r1, 0x50
    mtlr r0
    blr
