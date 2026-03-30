    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    beq @L_8013B834
    mr r31, r3
    li r0, 0x0
    sth r0, 0xcc(r3)
    li r4, 0x0
    li r5, 0x24
    bl memset
    lwz r4, 0x54(r31)
    cmpwi r4, 0x0
    beq @L_8013B834
    lwz r3, 0x50(r31)
    bl fn_800F9318
    mr. r30, r3
    beq @L_8013B834
    bl fn_800B856C
    lhz r5, 0x4e(r31)
    li r4, 0x20
    lhz r3, 0x4c(r31)
    addi r0, r5, 0x1
    stw r30, 0x0(r31)
    addi r3, r3, 0x1
    sth r3, 0x1c(r31)
    sth r0, 0x1e(r31)
    lhz r6, 0x1c(r31)
    lhz r0, 0x1e(r31)
    slwi r3, r6, 4
    mullw r7, r6, r0
    addi r0, r3, 0x3
    slwi r6, r7, 2
    slwi r3, r7, 3
    mullw r5, r5, r0
    addi r6, r6, 0x1f
    addi r0, r3, 0x1f
    clrrwi r28, r6, 5
    addi r3, r5, 0x1f
    clrrwi r5, r3, 5
    stw r5, 0x18(r31)
    mulli r7, r7, 0xc
    clrrwi r3, r0, 5
    lwz r5, 0x18(r31)
    addi r0, r7, 0x1f
    clrrwi r27, r0, 5
    add r0, r27, r28
    add r0, r0, r3
    add r26, r0, r5
    add r26, r27, r26
    mr r3, r26
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r29, r3
    beq @L_8013B718
    bl fn_800E27B0
    sth r29, 0x20(r31)
    mr r29, r3
    mr r5, r26
    li r4, 0x0
    bl memset
    stw r29, 0x14(r31)
    li r3, 0x0
    li r4, 0x0
    lwz r0, 0x18(r31)
    add r0, r29, r0
    stw r0, 0x4(r31)
    add r0, r0, r27
    stw r0, 0x8(r31)
    add r0, r0, r27
    stw r0, 0x10(r31)
    add r0, r0, r28
    stw r0, 0xc(r31)
    bl fn_800E0E14
    li r0, 0x1
    b @L_8013B71C
    @L_8013B718:
    li r0, 0x0
    @L_8013B71C:
    clrlwi. r0, r0, 24
    beq @L_8013B834
    lfs f1, 0x40(r31)
    mr r3, r31
    lfs f2, 0x44(r31)
    addi r4, r31, 0x24
    lfs f3, 0x48(r31)
    addi r5, r31, 0x3c
    bl fn_8013BE04
    mr r3, r30
    mr r4, r31
    bl fn_8013C074
    clrlwi. r0, r3, 24
    beq @L_8013B7F4
    lhz r3, 0x4e(r31)
    addi r0, r3, 0x1
    mulli r3, r0, 0xc
    bl fn_800E3534
    clrlwi. r0, r3, 16
    beq @L_8013B780
    sth r3, 0xc0(r31)
    bl fn_800E27B0
    stw r3, 0xbc(r31)
    li r0, 0x1
    b @L_8013B784
    @L_8013B780:
    li r0, 0x0
    @L_8013B784:
    clrlwi. r0, r0, 24
    beq @L_8013B7F4
    mr r3, r30
    li r4, 0x0
    bl fn_800E4014
    mr r3, r30
    addi r4, r31, 0x30
    bl fn_800E4170
    mr r3, r30
    bl fn_800EC1B0
    clrlwi. r0, r3, 24
    beq @L_8013B7EC
    mr r3, r30
    li r4, 0x0
    bl fn_800EC35C
    lfs f1, lbl_8047D200(r2)
    mr r3, r30
    bl fn_800EC308
    lfs f1, lbl_8047D204(r2)
    mr r3, r30
    bl fn_800EC2A4
    mr r3, r30
    li r4, 0x1
    bl fn_800EC208
    mr r3, r30
    bl fn_800EC1E4
    @L_8013B7EC:
    li r3, 0x1
    b @L_8013B848
    @L_8013B7F4:
    lhz r26, 0x20(r31)
    li r0, 0x0
    sth r0, 0x20(r31)
    mr r3, r26
    bl fn_800E24B0
    mr r3, r26
    bl fn_800E209C
    lhz r26, 0xc0(r31)
    li r0, 0x0
    cmplwi r26, 0x0
    sth r0, 0xc0(r31)
    beq @L_8013B834
    mr r3, r26
    bl fn_800E24B0
    mr r3, r26
    bl fn_800E209C
    @L_8013B834:
    lis r3, lbl_80272EA0@ha
    addi r3, r3, lbl_80272EA0@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013B848:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
