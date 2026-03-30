    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r6
    lwz r0, 0x14(r3)
    rlwinm. r0, r0, 0, 27, 27
    bne @L_80197984
    lwz r3, 0x14(r28)
    rlwinm r0, r5, 18, 0, 13
    and. r31, r3, r0
    beq @L_80197984
    cmplwi r28, 0x0
    beq @L_8019780C
    cmplwi r28, 0x0
    bne @L_801977DC
    addi r3, r2, -23744
    li r4, 0x25d
    addi r5, r2, -23736
    bl fn_80196E10
@L_801977DC:
    lwz r0, 0x14(r28)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801977FC
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801977FC
    li r3, 0x1
@L_801977FC:
    cmpwi r3, 0x0
    beq @L_8019780C
    mr r3, r28
    bl fn_8019D9DC
@L_8019780C:
    cmplwi r29, 0x0
    bne @L_80197820
    bl fn_801942B8
    addi r0, r3, 0x54
    mr r29, r0
@L_80197820:
    lwz r6, 0x0(r28)
    mr r3, r28
    mr r4, r29
    addi r5, r1, 0x8
    lwz r12, 0x44(r6)
    mtctr r12
    bctrl
    rlwinm. r0, r31, 0, 13, 13
    beq @L_80197868
    lwz r5, 0x0(r28)
    mr r3, r28
    mr r4, r29
    mr r7, r30
    lwz r12, 0x48(r5)
    addi r5, r1, 0x8
    li r6, 0x1
    mtctr r12
    bctrl
@L_80197868:
    lwz r0, lbl_8047B244(r13)
    cmpwi r0, 0x0
    bne @L_801978D0
    rlwinm. r0, r31, 0, 11, 11
    beq @L_801978A0
    lwz r5, 0x0(r28)
    mr r3, r28
    mr r4, r29
    mr r7, r30
    lwz r12, 0x48(r5)
    addi r5, r1, 0x8
    li r6, 0x4
    mtctr r12
    bctrl
@L_801978A0:
    rlwinm. r0, r31, 0, 12, 12
    beq @L_80197984
    lwz r5, 0x0(r28)
    mr r3, r28
    mr r4, r29
    mr r7, r30
    lwz r12, 0x48(r5)
    addi r5, r1, 0x8
    li r6, 0x2
    mtctr r12
    bctrl
    b @L_80197984
@L_801978D0:
    rlwinm. r0, r31, 0, 11, 12
    beq @L_80197984
    lis r3, lbl_80465348@ha
    addi r3, r3, lbl_80465348@l
    bl fn_801AA4CC
    mr r27, r3
    li r4, 0x0
    li r5, 0x18
    addi r3, r27, 0x30
    bl memset
    addi r3, r1, 0x8
    mr r4, r27
    bl fn_800A2D64
    cmplwi r29, 0x0
    beq @L_80197924
    bl fn_801A8524
    mr r0, r3
    mr r3, r29
    stw r0, 0x30(r27)
    lwz r4, 0x30(r27)
    bl fn_800A2D64
@L_80197924:
    stw r28, 0x34(r27)
    rlwinm. r0, r31, 0, 11, 11
    addi r0, r27, 0x44
    stw r30, 0x38(r27)
    lwz r3, lbl_80478C64(r13)
    stw r27, 0x0(r3)
    stw r0, lbl_80478C64(r13)
    beq @L_80197960
    lwz r3, lbl_80478C68(r13)
    addi r0, r27, 0x3c
    stw r27, 0x0(r3)
    lwz r3, lbl_8047B254(r13)
    stw r0, lbl_80478C68(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B254(r13)
@L_80197960:
    rlwinm. r0, r31, 0, 12, 12
    beq @L_80197984
    lwz r3, lbl_80478C6C(r13)
    addi r0, r27, 0x40
    stw r27, 0x0(r3)
    lwz r3, lbl_8047B25C(r13)
    stw r0, lbl_80478C6C(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B25C(r13)
@L_80197984:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
