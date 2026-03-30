    nofralloc
    stwu r1, -0x240(r1)
    mflr r0
    lis r7, -32713
    stw r0, 0x244(r1)
    li r0, 0x5
    stmw r25, 0x224(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    addi r31, r7, -11392
    mr r28, r6
    addi r3, r1, 0x1c
    li r4, 0x0
    li r5, 0x80
    stw r0, 0x18(r1)
    bl memset
    li r9, 0x0
    b @L_801B7D50
@L_801B7CE8:
    rlwinm r0, r9, 2, 0, 29
    addi r3, r1, 0x19c
    li r4, 0x0
    li r8, 0x0
    stwx r4, r3, r0
    b @L_801B7D38
@L_801B7D00:
    mulli r3, r9, 0x28
    rlwinm r0, r8, 2, 0, 29
    rlwinm r7, r9, 2, 0, 29
    addi r6, r1, 0x19c
    add r3, r26, r3
    lwzx r5, r6, r7
    add r3, r3, r0
    li r4, 0x1
    lwz r3, 0x8(r3)
    addi r8, r8, 0x1
    lbz r0, 0x4(r3)
    slw r0, r4, r0
    or r0, r5, r0
    stwx r0, r6, r7
@L_801B7D38:
    mulli r0, r9, 0x28
    add r3, r26, r0
    lbz r0, 0x5(r3)
    cmpw r8, r0
    blt @L_801B7D00
    addi r9, r9, 0x1
@L_801B7D50:
    cmpw r9, r25
    blt @L_801B7CE8
    li r5, 0x0
    b @L_801B7D7C
@L_801B7D60:
    rlwinm r0, r5, 2, 0, 29
    addi r3, r1, 0x19c
    lwzx r4, r3, r0
    rlwinm r0, r5, 2, 0, 29
    addi r3, r1, 0x11c
    addi r5, r5, 0x1
    stwx r4, r3, r0
@L_801B7D7C:
    cmpw r5, r25
    blt @L_801B7D60
@L_801B7D84:
    li r10, 0x0
    li r11, 0x0
    b @L_801B7E04
@L_801B7D90:
    rlwinm r0, r11, 2, 0, 29
    addi r3, r1, 0x11c
    li r4, 0x1
    lwzx r8, r3, r0
    slw r9, r4, r11
    li r12, 0x0
    b @L_801B7DF8
@L_801B7DAC:
    rlwinm r0, r12, 2, 0, 29
    addi r3, r1, 0x11c
    lwzx r0, r3, r0
    and. r0, r9, r0
    beq @L_801B7DF4
    rlwinm r0, r12, 2, 0, 29
    addi r3, r1, 0x11c
    lwzx r7, r3, r0
    rlwinm r5, r12, 2, 0, 29
    addi r4, r1, 0x11c
    rlwinm r0, r12, 2, 0, 29
    or r6, r7, r8
    addi r3, r1, 0x11c
    stwx r6, r4, r5
    lwzx r0, r3, r0
    cmplw r7, r0
    beq @L_801B7DF4
    li r10, 0x1
@L_801B7DF4:
    addi r12, r12, 0x1
@L_801B7DF8:
    cmpw r12, r25
    blt @L_801B7DAC
    addi r11, r11, 0x1
@L_801B7E04:
    cmpw r11, r25
    blt @L_801B7D90
    cmpwi r10, 0x0
    bne @L_801B7D84
    addi r0, r1, 0x9c
    addi r4, r1, 0x18
    stw r0, 0x8(r1)
    addi r0, r1, 0x1c
    mr r3, r25
    mr r6, r26
    stw r4, 0xc(r1)
    addi r4, r1, 0x19c
    addi r5, r1, 0x11c
    li r7, 0x0
    stw r0, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_801B89BC
    li r30, 0x0
    b @L_801B8008
@L_801B7E58:
    rlwinm r0, r30, 2, 0, 29
    addi r3, r1, 0x1c
    lwzx r4, r3, r0
    rlwinm r3, r30, 2, 0, 29
    rlwinm r0, r30, 2, 0, 29
    mulli r4, r4, 0x28
    lwzx r4, r26, r4
    stwx r4, r27, r3
    lwzx r3, r27, r0
    lbz r0, 0xc(r3)
    cmplwi r0, 0xff
    beq @L_801B7F68
    rlwinm r0, r30, 2, 0, 29
    li r4, 0x3
    lwzx r3, r27, r0
    li r29, 0x0
    lbz r0, 0xc(r3)
    rlwinm r0, r0, 1, 0, 30
    add r3, r28, r0
    stb r4, 0x14(r3)
    b @L_801B7F60
@L_801B7EAC:
    rlwinm r3, r30, 2, 0, 29
    rlwinm r0, r29, 3, 0, 28
    lwzx r3, r27, r3
    add r3, r3, r0
    lwz r3, 0x38(r3)
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801B7F5C
    rlwinm r3, r30, 2, 0, 29
    rlwinm r0, r29, 3, 0, 28
    lwzx r3, r27, r3
    add r3, r3, r0
    lbz r0, 0x35(r3)
    cmplwi r0, 0x1
    bne @L_801B7F24
    rlwinm r3, r30, 2, 0, 29
    rlwinm r5, r29, 3, 0, 28
    lwzx r6, r27, r3
    rlwinm r0, r30, 2, 0, 29
    lwzx r3, r27, r0
    rlwinm r0, r29, 3, 0, 28
    add r5, r6, r5
    addi r4, r31, 0x0
    lwz r5, 0x38(r5)
    add r3, r3, r0
    lbz r0, 0xc(r5)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    stb r0, 0x36(r3)
    b @L_801B7F5C
@L_801B7F24:
    rlwinm r3, r30, 2, 0, 29
    rlwinm r5, r29, 3, 0, 28
    lwzx r6, r27, r3
    rlwinm r0, r30, 2, 0, 29
    lwzx r3, r27, r0
    rlwinm r0, r29, 3, 0, 28
    add r5, r6, r5
    addi r4, r31, 0x10
    lwz r5, 0x38(r5)
    add r3, r3, r0
    lbz r0, 0x18(r5)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    stb r0, 0x36(r3)
@L_801B7F5C:
    addi r29, r29, 0x1
@L_801B7F60:
    cmpwi r29, 0x4
    blt @L_801B7EAC
@L_801B7F68:
    rlwinm r0, r30, 2, 0, 29
    lwzx r3, r27, r0
    lbz r0, 0x18(r3)
    cmplwi r0, 0xff
    beq @L_801B8004
    rlwinm r0, r30, 2, 0, 29
    li r4, 0x1
    lwzx r3, r27, r0
    li r29, 0x0
    lbz r0, 0x18(r3)
    rlwinm r0, r0, 1, 0, 30
    add r3, r28, r0
    stb r4, 0x15(r3)
    b @L_801B7FFC
@L_801B7FA0:
    rlwinm r3, r30, 2, 0, 29
    rlwinm r0, r29, 3, 0, 28
    lwzx r3, r27, r3
    add r3, r3, r0
    lwz r3, 0x58(r3)
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801B7FF8
    rlwinm r3, r30, 2, 0, 29
    rlwinm r5, r29, 3, 0, 28
    lwzx r6, r27, r3
    rlwinm r0, r30, 2, 0, 29
    lwzx r3, r27, r0
    rlwinm r0, r29, 3, 0, 28
    add r5, r6, r5
    addi r4, r31, 0x20
    lwz r5, 0x58(r5)
    add r3, r3, r0
    lbz r0, 0x18(r5)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    stb r0, 0x56(r3)
@L_801B7FF8:
    addi r29, r29, 0x1
@L_801B7FFC:
    cmpwi r29, 0x4
    blt @L_801B7FA0
@L_801B8004:
    addi r30, r30, 0x1
@L_801B8008:
    cmpw r30, r25
    blt @L_801B7E58
    lmw r25, 0x224(r1)
    lwz r0, 0x244(r1)
    mtlr r0
    addi r1, r1, 0x240
    blr
