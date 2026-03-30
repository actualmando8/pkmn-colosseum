    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    li r0, 0x0
    stmw r27, 0x1c(r1)
    stw r0, 0x8(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205184
    mr r0, r3
    li r3, 0x12
    mr r27, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r27
    mr r5, r0
    mr r4, r31
    bl fn_80229934
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218088
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_802181C4
@L_80218088:
    lis r3, -32734
    addi r5, r1, 0x8
    addi r4, r3, -32296
    li r6, 0x0
    li r3, 0x0
    bl fn_801F37B0
    cmplwi r31, 0x0
    beq @L_802181A0
    mr r6, r31
    li r3, 0x0
    li r4, 0x2e
    li r5, 0x0
    bl fn_801F3624
    rlwinm. r29, r3, 0, 24, 31
    beq @L_802181A0
    mr r3, r31
    bl fn_80205B8C
    li r4, 0xc3
    mr r28, r3
    bl fn_80123B5C
    mr r30, r3
    extsb. r0, r30
    blt @L_802181A0
    extsb r27, r30
    mr r3, r28
    mr r6, r27
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    subfc r5, r0, r29
    mr r6, r27
    subf r0, r29, r0
    li r4, 0x0
    subfe r7, r5, r5
    li r5, 0x80
    rlwinm r0, r0, 0, 24, 31
    and r0, r0, r7
    rlwinm r7, r0, 0, 24, 31
    bl fn_801254B4
    mr r3, r31
    rlwinm r28, r30, 0, 24, 31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802181A0
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802181A0
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    li r0, 0x1
    slw r0, r0, r28
    and. r0, r3, r0
    bne @L_802181A0
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802181A0
    mr r3, r31
    mr r5, r28
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_802181A0:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    bne @L_802181B8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    b @L_802181C0
@L_802181B8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
@L_802181C0:
    stw r0, lbl_8047B610(r13)
@L_802181C4:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
