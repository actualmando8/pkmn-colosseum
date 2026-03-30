    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_802016A4
    mr r30, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F0134
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_802016A4
    mr r31, r3
    mr r3, r29
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc
    bne @L_8021A904
    lis r3, lbl_8037984D@ha
    addi r0, r3, lbl_8037984D@l
    stw r0, lbl_8047B610(r13)
    b @L_8021A970
@L_8021A904:
    rlwinm r3, r30, 0, 24, 31
    rlwinm r0, r31, 0, 24, 31
    cmplw r3, r0
    beq @L_8021A944
    mr r3, r29
    li r4, 0xa
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021A944
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8021A944
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021A954
@L_8021A944:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021A970
@L_8021A954:
    mr r3, r29
    mr r5, r28
    li r4, 0xa
    bl fn_8020248C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021A970:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
