    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r11, 0x1
    stw r0, 0x54(r1)
    stmw r21, 0x24(r1)
    mr r25, r7
    mr r0, r25
    mr r21, r3
    addi r25, r25, 0x1
    li r7, 0x1
    mr r26, r9
    slw r9, r11, r8
    lwz r28, 0x58(r1)
    cmpw r25, r21
    rlwinm r3, r8, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    slw r7, r7, r8
    mr r27, r10
    lwz r29, 0x5c(r1)
    mr r22, r4
    lwz r30, 0x60(r1)
    mr r23, r5
    mr r24, r6
    or r26, r26, r9
    andc r27, r27, r7
    stwx r3, r28, r0
    bne @L_801B8A74
    mr r3, r21
    mr r4, r22
    mr r5, r24
    mr r6, r28
    bl fn_801B8B84
    lwz r0, 0x0(r29)
    cmpw r3, r0
    bge @L_801B8B70
    stw r3, 0x0(r29)
    li r31, 0x0
    b @L_801B8A68
@L_801B8A54:
    rlwinm r3, r31, 2, 0, 29
    rlwinm r0, r31, 2, 0, 29
    lwzx r3, r28, r3
    addi r31, r31, 0x1
    stwx r3, r30, r0
@L_801B8A68:
    cmpw r31, r21
    blt @L_801B8A54
    b @L_801B8B70
@L_801B8A74:
    rlwinm r0, r8, 2, 0, 29
    li r4, 0x0
    lwzx r0, r22, r0
    li r31, 0x0
    or r3, r27, r0
    b @L_801B8AAC
@L_801B8A8C:
    li r0, 0x1
    slw r0, r0, r31
    and. r0, r3, r0
    beq @L_801B8AA8
    rlwinm r0, r31, 2, 0, 29
    lwzx r0, r23, r0
    or r4, r4, r0
@L_801B8AA8:
    addi r31, r31, 0x1
@L_801B8AAC:
    cmpw r31, r21
    blt @L_801B8A8C
    mulli r0, r8, 0x28
    andc r27, r3, r4
    add r3, r24, r0
    lbz r0, 0x5(r3)
    cmplwi r0, 0x1
    bne @L_801B8B1C
    rlwinm r0, r8, 2, 0, 29
    lwzx r0, r22, r0
    and. r0, r27, r0
    beq @L_801B8B1C
    stw r28, 0x8(r1)
    mulli r0, r8, 0x28
    mr r3, r21
    stw r29, 0xc(r1)
    mr r4, r22
    add r8, r24, r0
    mr r5, r23
    stw r30, 0x10(r1)
    mr r6, r24
    mr r7, r25
    mr r9, r26
    lwz r8, 0x8(r8)
    mr r10, r27
    lbz r8, 0x4(r8)
    bl fn_801B89BC
    b @L_801B8B70
@L_801B8B1C:
    li r31, 0x0
    b @L_801B8B68
@L_801B8B24:
    li r0, 0x1
    slw r0, r0, r31
    and. r0, r27, r0
    beq @L_801B8B64
    stw r28, 0x8(r1)
    mr r3, r21
    mr r4, r22
    mr r5, r23
    stw r29, 0xc(r1)
    mr r6, r24
    mr r7, r25
    mr r8, r31
    stw r30, 0x10(r1)
    mr r9, r26
    mr r10, r27
    bl fn_801B89BC
@L_801B8B64:
    addi r31, r31, 0x1
@L_801B8B68:
    cmpw r31, r21
    blt @L_801B8B24
@L_801B8B70:
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
