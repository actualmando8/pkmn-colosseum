    nofralloc
    mflr r0
    lis r3, lbl_803FC5D0@ha
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r25, 0x1c(r1)
    addi r31, r3, lbl_803FC5D0@l
    lwz r0, lbl_8047A8A0(r13)
    cmpwi r0, 0x0
    beq @L_800AAF64
    li r3, 0x1
    b @L_800AB13C
    @L_800AAF64:
    lwz r3, lbl_80478A08(r13)
    bl OSRegisterVersion
    lwz r3, __PADSpec(r13)
    cmplwi r3, 0x0
    beq @L_800AAF7C
    bl fn_800AB5B4
    @L_800AAF7C:
    lwz r0, lbl_8047AA58(r13)
    li r3, 0x1
    stw r3, lbl_8047A8A0(r13)
    cmplwi r0, 0x0
    beq @L_800AB018
    bl OSGetTime
    addi r25, r4, 0x0
    addi r26, r3, 0x0
    li r5, 0x10
    bl fn_800C4C98
    lis r5, 0x1
    subi r27, r5, 0x1
    li r28, 0x0
    and r6, r4, r27
    and r4, r25, r27
    and r5, r3, r28
    and r0, r26, r28
    addc r29, r4, r6
    addi r3, r26, 0x0
    addi r4, r25, 0x0
    adde r30, r0, r5
    li r5, 0x20
    bl fn_800C4C98
    and r4, r4, r27
    and r0, r3, r28
    addc r29, r4, r29
    addi r3, r26, 0x0
    addi r4, r25, 0x0
    adde r30, r0, r30
    li r5, 0x30
    bl fn_800C4C98
    and r0, r4, r27
    addc r5, r0, r29
    lis r0, 0xf000
    li r4, 0x3fff
    stw r0, lbl_8047A8AC(r13)
    and r0, r5, r4
    lis r3, 0x8000
    sth r0, 0x30e0(r3)
    @L_800AB018:
    lis r3, 0x8000
    lhz r0, 0x30e0(r3)
    clrlslwi r0, r0, 18, 8
    oris r0, r0, 0x4d00
    stw r0, 0x40(r31)
    lhz r0, 0x30e0(r3)
    clrlslwi r0, r0, 18, 8
    oris r0, r0, 0x4d40
    stw r0, 0x44(r31)
    lhz r0, 0x30e0(r3)
    clrlslwi r0, r0, 18, 8
    oris r0, r0, 0x4d80
    stw r0, 0x48(r31)
    lhz r0, 0x30e0(r3)
    clrlslwi r0, r0, 18, 8
    oris r0, r0, 0x4dc0
    stw r0, 0x4c(r31)
    bl fn_800D104C
    lis r3, lbl_80312500@ha
    addi r3, r3, lbl_80312500@l
    bl OSRegisterResetFunction
    lis r28, 0xf000
    bl OSDisableInterrupts
    lwz r4, lbl_8047A8B8(r13)
    li r7, 0x0
    lwz r5, lbl_8047A8B0(r13)
    addi r27, r3, 0x0
    lwz r0, lbl_8047A8B4(r13)
    or r28, r28, r4
    lwz r4, lbl_8047A8A8(r13)
    or r5, r5, r0
    lwz r0, lbl_80478A18(r13)
    andc r28, r28, r5
    lwz r5, lbl_8047A8A4(r13)
    or r6, r4, r28
    stw r7, lbl_8047A8B8(r13)
    andc r4, r5, r28
    stw r6, lbl_8047A8A8(r13)
    cmplwi r0, 0x4
    lwz r0, lbl_8047A8A8(r13)
    stw r4, lbl_8047A8A4(r13)
    and r3, r0, r5
    bne @L_800AB0D0
    lwz r0, lbl_8047A8AC(r13)
    or r0, r0, r28
    stw r0, lbl_8047A8AC(r13)
    @L_800AB0D0:
    bl fn_800D0464
    lwz r0, lbl_80478A0C(r13)
    cmpwi r0, 0x20
    bne @L_800AB130
    lwz r5, lbl_8047A8A8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    lwz r4, lbl_80478A0C(r13)
    cmpwi r4, 0x20
    beq @L_800AB130
    lis r3, 0x8000
    mulli r0, r4, 0xc
    srw r3, r3, r4
    andc r4, r5, r3
    add r3, r31, r0
    stw r4, lbl_8047A8A8(r13)
    li r4, 0x0
    li r5, 0xc
    addi r3, r3, 0x10
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    @L_800AB130:
    mr r3, r27
    bl OSRestoreInterrupts
    li r3, 0x1
    @L_800AB13C:
    lmw r25, 0x1c(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
