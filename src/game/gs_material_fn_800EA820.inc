    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r26, 0x38(r1)
    mr r28, r3
    mr r26, r4
    mr r29, r6
    mr r30, r7
    mr r31, r8
    lwz r3, 0x14(r3)
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA94C
    slwi r0, r5, 18
    and. r27, r3, r0
    beq @L_800EA94C
    cmplwi r28, 0x0
    beq @L_800EA8A4
    bne @L_800EA878
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EA878:
    lwz r4, 0x14(r28)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EA894
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EA894
    li r3, 0x1
    @L_800EA894:
    cmpwi r3, 0x0
    beq @L_800EA8A4
    mr r3, r28
    bl fn_8019D9DC
    @L_800EA8A4:
    cmplwi r26, 0x0
    bne @L_800EA8C4
    lis r3, lbl_804016A0@ha
    addi r3, r3, lbl_804016A0@l
    bl fn_800E064C
    lis r3, lbl_804016A0@ha
    addi r0, r3, lbl_804016A0@l
    mr r26, r0
    @L_800EA8C4:
    mr r3, r28
    mr r4, r26
    addi r5, r1, 0x8
    bl fn_80197B6C
    rlwinm. r0, r27, 0, 13, 13
    beq @L_800EA8FC
    mr r3, r28
    mr r4, r26
    mr r7, r29
    mr r8, r30
    mr r9, r31
    addi r5, r1, 0x8
    li r6, 0x1
    bl fn_800EA960
    @L_800EA8FC:
    rlwinm. r0, r27, 0, 11, 11
    beq @L_800EA924
    mr r3, r28
    mr r4, r26
    mr r7, r29
    mr r8, r30
    mr r9, r31
    addi r5, r1, 0x8
    li r6, 0x4
    bl fn_800EA960
    @L_800EA924:
    rlwinm. r0, r27, 0, 12, 12
    beq @L_800EA94C
    mr r3, r28
    mr r4, r26
    mr r7, r29
    mr r8, r30
    mr r9, r31
    addi r5, r1, 0x8
    li r6, 0x2
    bl fn_800EA960
    @L_800EA94C:
    lmw r26, 0x38(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
