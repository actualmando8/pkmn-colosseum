    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    li r0, 0x0
    stmw r27, 0x7c(r1)
    mr. r27, r3
    li r3, 0x0
    mr r28, r4
    mr r29, r5
    stw r3, 0x74(r1)
    stw r0, 0x70(r1)
    beq @L_8019F6E4
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F440
    lwz r30, 0x10(r27)
    li r3, 0x0
    li r0, 0x0
    cmplwi r30, 0x0
    stw r3, 0x58(r1)
    stw r0, 0x5c(r1)
    beq @L_8019F424
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F2F4
    lwz r30, 0x10(r30)
    li r3, 0x0
    li r0, 0x0
    cmplwi r30, 0x0
    stw r3, 0x38(r1)
    stw r0, 0x3c(r1)
    beq @L_8019F2D8
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F264
    lwz r3, 0x10(r30)
    addi r4, r1, 0x38
    addi r5, r1, 0x3c
    bl fn_8019F1C4
    b @L_8019F2D8
    @L_8019F264:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F294
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F294
    lwz r3, 0x18(r30)
    addi r4, r1, 0x38
    addi r5, r1, 0x3c
    bl fn_80199178
    @L_8019F294:
    lwz r30, 0x10(r30)
    b @L_8019F2D0
    @L_8019F29C:
    mr r3, r30
    addi r4, r1, 0x40
    addi r5, r1, 0x44
    bl fn_8019F1C4
    lwz r5, 0x38(r1)
    lwz r4, 0x40(r1)
    lwz r3, 0x3c(r1)
    lwz r0, 0x44(r1)
    add r4, r5, r4
    stw r4, 0x38(r1)
    add r0, r3, r0
    stw r0, 0x3c(r1)
    lwz r30, 0x8(r30)
    @L_8019F2D0:
    cmplwi r30, 0x0
    bne @L_8019F29C
    @L_8019F2D8:
    addic. r0, r1, 0x58
    beq @L_8019F2E8
    lwz r0, 0x38(r1)
    stw r0, 0x58(r1)
    @L_8019F2E8:
    lwz r0, 0x3c(r1)
    stw r0, 0x5c(r1)
    b @L_8019F424
    @L_8019F2F4:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F324
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F324
    lwz r3, 0x18(r30)
    addi r4, r1, 0x58
    addi r5, r1, 0x5c
    bl fn_80199178
    @L_8019F324:
    lwz r31, 0x10(r30)
    b @L_8019F41C
    @L_8019F32C:
    li r3, 0x0
    li r0, 0x0
    cmplwi r31, 0x0
    stw r3, 0x28(r1)
    stw r0, 0x2c(r1)
    beq @L_8019F3D8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F364
    lwz r3, 0x10(r31)
    addi r4, r1, 0x28
    addi r5, r1, 0x2c
    bl fn_8019F1C4
    b @L_8019F3D8
    @L_8019F364:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F394
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F394
    lwz r3, 0x18(r31)
    addi r4, r1, 0x28
    addi r5, r1, 0x2c
    bl fn_80199178
    @L_8019F394:
    lwz r30, 0x10(r31)
    b @L_8019F3D0
    @L_8019F39C:
    mr r3, r30
    addi r4, r1, 0x30
    addi r5, r1, 0x34
    bl fn_8019F1C4
    lwz r5, 0x28(r1)
    lwz r4, 0x30(r1)
    lwz r3, 0x2c(r1)
    lwz r0, 0x34(r1)
    add r4, r5, r4
    stw r4, 0x28(r1)
    add r0, r3, r0
    stw r0, 0x2c(r1)
    lwz r30, 0x8(r30)
    @L_8019F3D0:
    cmplwi r30, 0x0
    bne @L_8019F39C
    @L_8019F3D8:
    addic. r0, r1, 0x60
    beq @L_8019F3E8
    lwz r0, 0x28(r1)
    stw r0, 0x60(r1)
    @L_8019F3E8:
    addic. r0, r1, 0x64
    beq @L_8019F3F8
    lwz r0, 0x2c(r1)
    stw r0, 0x64(r1)
    @L_8019F3F8:
    lwz r5, 0x58(r1)
    lwz r4, 0x60(r1)
    lwz r3, 0x5c(r1)
    lwz r0, 0x64(r1)
    add r4, r5, r4
    stw r4, 0x58(r1)
    add r0, r3, r0
    stw r0, 0x5c(r1)
    lwz r31, 0x8(r31)
    @L_8019F41C:
    cmplwi r31, 0x0
    bne @L_8019F32C
    @L_8019F424:
    addic. r0, r1, 0x74
    beq @L_8019F434
    lwz r0, 0x58(r1)
    stw r0, 0x74(r1)
    @L_8019F434:
    lwz r0, 0x5c(r1)
    stw r0, 0x70(r1)
    b @L_8019F6E4
    @L_8019F440:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F470
    lwz r0, 0x14(r27)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F470
    lwz r3, 0x18(r27)
    addi r4, r1, 0x74
    addi r5, r1, 0x70
    bl fn_80199178
    @L_8019F470:
    lwz r30, 0x10(r27)
    b @L_8019F6DC
    @L_8019F478:
    li r3, 0x0
    li r0, 0x0
    cmplwi r30, 0x0
    stw r3, 0x48(r1)
    stw r0, 0x4c(r1)
    beq @L_8019F698
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F568
    lwz r31, 0x10(r30)
    li r3, 0x0
    li r0, 0x0
    cmplwi r31, 0x0
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    beq @L_8019F54C
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F4D8
    lwz r3, 0x10(r31)
    addi r4, r1, 0x18
    addi r5, r1, 0x1c
    bl fn_8019F1C4
    b @L_8019F54C
    @L_8019F4D8:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F508
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F508
    lwz r3, 0x18(r31)
    addi r4, r1, 0x18
    addi r5, r1, 0x1c
    bl fn_80199178
    @L_8019F508:
    lwz r31, 0x10(r31)
    b @L_8019F544
    @L_8019F510:
    mr r3, r31
    addi r4, r1, 0x20
    addi r5, r1, 0x24
    bl fn_8019F1C4
    lwz r5, 0x18(r1)
    lwz r4, 0x20(r1)
    lwz r3, 0x1c(r1)
    lwz r0, 0x24(r1)
    add r4, r5, r4
    stw r4, 0x18(r1)
    add r0, r3, r0
    stw r0, 0x1c(r1)
    lwz r31, 0x8(r31)
    @L_8019F544:
    cmplwi r31, 0x0
    bne @L_8019F510
    @L_8019F54C:
    addic. r0, r1, 0x48
    beq @L_8019F55C
    lwz r0, 0x18(r1)
    stw r0, 0x48(r1)
    @L_8019F55C:
    lwz r0, 0x1c(r1)
    stw r0, 0x4c(r1)
    b @L_8019F698
    @L_8019F568:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F598
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F598
    lwz r3, 0x18(r30)
    addi r4, r1, 0x48
    addi r5, r1, 0x4c
    bl fn_80199178
    @L_8019F598:
    lwz r31, 0x10(r30)
    b @L_8019F690
    @L_8019F5A0:
    li r3, 0x0
    li r0, 0x0
    cmplwi r31, 0x0
    stw r3, 0x8(r1)
    stw r0, 0xc(r1)
    beq @L_8019F64C
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019F5D8
    lwz r3, 0x10(r31)
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    bl fn_8019F1C4
    b @L_8019F64C
    @L_8019F5D8:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_8019F608
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019F608
    lwz r3, 0x18(r31)
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    bl fn_80199178
    @L_8019F608:
    lwz r27, 0x10(r31)
    b @L_8019F644
    @L_8019F610:
    mr r3, r27
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    bl fn_8019F1C4
    lwz r5, 0x8(r1)
    lwz r4, 0x10(r1)
    lwz r3, 0xc(r1)
    lwz r0, 0x14(r1)
    add r4, r5, r4
    stw r4, 0x8(r1)
    add r0, r3, r0
    stw r0, 0xc(r1)
    lwz r27, 0x8(r27)
    @L_8019F644:
    cmplwi r27, 0x0
    bne @L_8019F610
    @L_8019F64C:
    addic. r0, r1, 0x50
    beq @L_8019F65C
    lwz r0, 0x8(r1)
    stw r0, 0x50(r1)
    @L_8019F65C:
    addic. r0, r1, 0x54
    beq @L_8019F66C
    lwz r0, 0xc(r1)
    stw r0, 0x54(r1)
    @L_8019F66C:
    lwz r5, 0x48(r1)
    lwz r4, 0x50(r1)
    lwz r3, 0x4c(r1)
    lwz r0, 0x54(r1)
    add r4, r5, r4
    stw r4, 0x48(r1)
    add r0, r3, r0
    stw r0, 0x4c(r1)
    lwz r31, 0x8(r31)
    @L_8019F690:
    cmplwi r31, 0x0
    bne @L_8019F5A0
    @L_8019F698:
    addic. r0, r1, 0x6c
    beq @L_8019F6A8
    lwz r0, 0x48(r1)
    stw r0, 0x6c(r1)
    @L_8019F6A8:
    addic. r0, r1, 0x68
    beq @L_8019F6B8
    lwz r0, 0x4c(r1)
    stw r0, 0x68(r1)
    @L_8019F6B8:
    lwz r5, 0x74(r1)
    lwz r4, 0x6c(r1)
    lwz r3, 0x70(r1)
    lwz r0, 0x68(r1)
    add r4, r5, r4
    stw r4, 0x74(r1)
    add r0, r3, r0
    stw r0, 0x70(r1)
    lwz r30, 0x8(r30)
    @L_8019F6DC:
    cmplwi r30, 0x0
    bne @L_8019F478
    @L_8019F6E4:
    cmplwi r28, 0x0
    beq @L_8019F6F4
    lwz r0, 0x74(r1)
    stw r0, 0x0(r28)
    @L_8019F6F4:
    cmplwi r29, 0x0
    beq @L_8019F704
    lwz r0, 0x70(r1)
    stw r0, 0x0(r29)
    @L_8019F704:
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
