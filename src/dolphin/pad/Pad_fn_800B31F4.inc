    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    addi r29, r3, 0x0
    mulli r4, r29, 0x110
    stw r28, 0x20(r1)
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r3, r0, r4
    lwz r0, 0x24(r3)
    addi r31, r3, 0x0
    cmpwi r0, 0x0
    bne @L_800B34DC
    addi r3, r29, 0x0
    addi r5, r1, 0x18
    li r4, 0x0
    bl fn_80099400
    cmpwi r3, 0x0
    bne @L_800B3254
    li r30, -0x3
    b @L_800B3270
    @L_800B3254:
    lwz r3, 0x18(r1)
    bl fn_800B2FAC
    cmpwi r3, 0x0
    beq @L_800B326C
    li r30, 0x0
    b @L_800B3270
    @L_800B326C:
    li r30, -0x2
    @L_800B3270:
    cmpwi r30, 0x0
    blt @L_800B35B8
    lwz r0, 0x18(r1)
    lis r4, lbl_80312960@ha
    addi r4, r4, lbl_80312960@l
    stw r0, 0x108(r31)
    lis r3, lbl_80312980@ha
    addi r0, r3, lbl_80312980@l
    lwz r5, 0x18(r1)
    addi r3, r29, 0x0
    rlwinm r5, r5, 0, 24, 29
    sth r5, 0x8(r31)
    lwz r5, 0x18(r1)
    rlwinm r5, r5, 23, 27, 29
    add r4, r4, r5
    lwz r4, 0x0(r4)
    stw r4, 0xc(r31)
    lhz r5, 0x8(r31)
    lwz r4, 0xc(r31)
    slwi r5, r5, 20
    srawi r5, r5, 3
    addze r5, r5
    divw r4, r5, r4
    sth r4, 0x10(r31)
    lwz r4, 0x18(r1)
    rlwinm r4, r4, 26, 27, 29
    add r4, r0, r4
    lwz r0, 0x0(r4)
    stw r0, 0x14(r31)
    bl fn_800AF750
    mr. r30, r3
    blt @L_800B35B8
    addi r3, r29, 0x0
    addi r4, r1, 0x14
    bl fn_800AF660
    mr. r30, r3
    blt @L_800B35B8
    mr r3, r29
    bl fn_80098944
    cmpwi r3, 0x0
    bne @L_800B331C
    li r30, -0x3
    b @L_800B35B8
    @L_800B331C:
    lbz r0, 0x14(r1)
    rlwinm. r0, r0, 0, 25, 25
    bne @L_800B3438
    addi r3, r29, 0x0
    addi r4, r31, 0x18
    bl fn_800B089C
    mr. r30, r3
    blt @L_800B35B8
    bl __OSLockSramEx
    mulli r0, r29, 0xc
    add r4, r3, r0
    lbz r0, 0x18(r31)
    addi r5, r31, 0x18
    stb r0, 0x0(r4)
    addi r8, r5, 0x2
    addi r9, r5, 0x3
    lbz r7, 0x18(r31)
    addi r10, r5, 0x4
    lbz r0, 0x19(r31)
    addi r11, r5, 0x5
    addi r12, r5, 0x6
    stb r0, 0x1(r4)
    addi r28, r5, 0x7
    li r5, 0x8
    lbz r0, 0x19(r31)
    lbz r6, 0x0(r8)
    add r0, r7, r0
    stb r6, 0x2(r4)
    lbz r7, 0x0(r8)
    lbz r6, 0x0(r9)
    add r0, r0, r7
    stb r6, 0x3(r4)
    lbz r7, 0x0(r9)
    lbz r6, 0x0(r10)
    add r0, r0, r7
    stb r6, 0x4(r4)
    lbz r7, 0x0(r10)
    lbz r6, 0x0(r11)
    add r0, r0, r7
    stb r6, 0x5(r4)
    lbz r7, 0x0(r11)
    lbz r6, 0x0(r12)
    add r0, r0, r7
    stb r6, 0x6(r4)
    lbz r7, 0x0(r12)
    lbz r6, 0x0(r28)
    add r0, r0, r7
    stb r6, 0x7(r4)
    lbz r6, 0x0(r28)
    add r0, r0, r6
    b @L_800B35D4
    @L_800B33E8:
    subfic r4, r5, 0xc
    cmpwi r5, 0xc
    mtctr r4
    bge @L_800B341C
    @L_800B33F8:
    addi r7, r5, 0x18
    add r7, r31, r7
    lbz r4, 0x0(r7)
    addi r5, r5, 0x1
    stb r4, 0x0(r6)
    addi r6, r6, 0x1
    lbz r4, 0x0(r7)
    add r0, r0, r4
    bdnz @L_800B33F8
    @L_800B341C:
    add r3, r3, r29
    nor r0, r0, r0
    stb r0, 0x26(r3)
    li r3, 0x1
    bl __OSUnlockSramEx
    mr r3, r30
    b @L_800B35E4
    @L_800B3438:
    li r0, 0x1
    stw r0, 0x24(r31)
    bl __OSLockSramEx
    mulli r0, r29, 0xc
    addi r30, r3, 0x0
    add r5, r30, r0
    lbz r4, 0x0(r5)
    li r6, 0x8
    lbz r0, 0x1(r5)
    lbz r3, 0x2(r5)
    add r28, r4, r0
    lbz r0, 0x3(r5)
    add r28, r28, r3
    lbz r3, 0x4(r5)
    add r28, r28, r0
    lbz r0, 0x5(r5)
    add r28, r28, r3
    lbz r3, 0x6(r5)
    add r28, r28, r0
    lbz r0, 0x7(r5)
    add r28, r28, r3
    add r28, r28, r0
    b @L_800B35DC
    @L_800B3494:
    subfic r0, r6, 0xc
    cmpwi r6, 0xc
    mtctr r0
    bge @L_800B34B4
    @L_800B34A4:
    lbz r0, 0x0(r3)
    addi r3, r3, 0x1
    add r28, r28, r0
    bdnz @L_800B34A4
    @L_800B34B4:
    li r3, 0x0
    bl __OSUnlockSramEx
    add r3, r30, r29
    nor r0, r28, r28
    lbz r3, 0x26(r3)
    clrlwi r0, r0, 24
    cmplw r3, r0
    beq @L_800B34DC
    li r30, -0x5
    b @L_800B35B8
    @L_800B34DC:
    lwz r0, 0x24(r31)
    cmpwi r0, 0x1
    bne @L_800B356C
    lwz r3, 0x108(r31)
    addis r0, r3, 0x8000
    cmplwi r0, 0x4
    bne @L_800B3528
    bl __OSLockSramEx
    mulli r0, r29, 0xc
    lhzx r28, r3, r0
    li r3, 0x0
    bl __OSUnlockSramEx
    lhz r0, lbl_80478A58(r13)
    cmplwi r0, 0xffff
    beq @L_800B3520
    cmplw r28, r0
    beq @L_800B3528
    @L_800B3520:
    li r30, -0x2
    b @L_800B35B8
    @L_800B3528:
    li r0, 0x2
    stw r0, 0x24(r31)
    addi r3, r29, 0x0
    li r4, 0x1
    bl fn_800AF5A0
    mr. r30, r3
    blt @L_800B35B8
    lis r3, fn_800AF35C@ha
    addi r4, r3, fn_800AF35C@l
    addi r3, r29, 0x0
    bl fn_8009870C
    mr r3, r29
    bl EXIUnlock
    lis r4, 0x1
    lwz r3, 0x80(r31)
    subi r4, r4, 0x6000
    bl DCInvalidateRange
    @L_800B356C:
    lwz r4, 0x24(r31)
    lis r3, fn_800B3604@ha
    lwz r0, 0xc(r31)
    addi r7, r3, fn_800B3604@l
    subi r3, r4, 0x2
    mullw r4, r0, r3
    lwz r5, 0x80(r31)
    slwi r0, r3, 13
    add r6, r5, r0
    addi r3, r29, 0x0
    li r5, 0x2000
    bl fn_800B1864
    mr. r28, r3
    bge @L_800B35B0
    addi r3, r31, 0x0
    addi r4, r28, 0x0
    bl fn_800B02F4
    @L_800B35B0:
    mr r3, r28
    b @L_800B35E4
    @L_800B35B8:
    mr r3, r29
    bl EXIUnlock
    addi r3, r29, 0x0
    addi r4, r30, 0x0
    bl fn_800B38DC
    mr r3, r30
    b @L_800B35E4
    @L_800B35D4:
    addi r6, r4, 0x8
    b @L_800B33E8
    @L_800B35DC:
    addi r3, r5, 0x8
    b @L_800B3494
    @L_800B35E4:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
