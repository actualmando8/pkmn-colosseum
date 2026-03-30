    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r4, lbl_8031554C@ha
    stw r0, 0x54(r1)
    addi r4, r4, lbl_8031554C@l
    stmw r22, 0x28(r1)
    lwz r29, 0x4(r3)
    lwz r28, 0x8(r3)
    lhz r31, 0x1e(r3)
    addi r22, r29, 0xc
    lhz r30, 0x1c(r3)
    mr r3, r28
    bl fn_800E01D0
    li r23, 0x0
    li r26, 0x0
    b @L_8013BAF0
    @L_8013BAD8:
    mulli r0, r26, 0xc
    mr r4, r28
    add r3, r28, r0
    bl fn_800E01D0
    add r26, r26, r31
    addi r23, r23, 0x1
    @L_8013BAF0:
    cmpw r23, r30
    blt @L_8013BAD8
    lis r3, lbl_80315540@ha
    subi r27, r31, 0x1
    addi r26, r3, lbl_80315540@l
    li r23, 0x1
    b @L_8013BBB4
    @L_8013BB0C:
    mr r5, r29
    mr r29, r22
    mr r4, r22
    addi r3, r1, 0x14
    addi r28, r28, 0xc
    addi r22, r22, 0xc
    bl fn_800E0168
    mr r4, r22
    mr r5, r29
    addi r3, r1, 0x8
    bl fn_800E0168
    addi r3, r1, 0x14
    mr r5, r26
    mr r4, r3
    bl fn_800DFFCC
    addi r3, r1, 0x8
    mr r5, r26
    mr r4, r3
    bl fn_800DFFCC
    mr r3, r28
    addi r4, r1, 0x8
    addi r5, r1, 0x14
    bl fn_800E019C
    lfs f1, lbl_8047D200(r2)
    mr r3, r28
    mr r4, r28
    bl fn_800E013C
    mr r3, r28
    mr r4, r28
    bl fn_800E0060
    li r24, 0x0
    mr r25, r24
    b @L_8013BBA8
    @L_8013BB90:
    mulli r0, r25, 0xc
    mr r4, r28
    add r3, r28, r0
    bl fn_800E01D0
    add r25, r25, r31
    addi r24, r24, 0x1
    @L_8013BBA8:
    cmpw r24, r30
    blt @L_8013BB90
    addi r23, r23, 0x1
    @L_8013BBB4:
    cmpw r23, r27
    blt @L_8013BB0C
    lis r4, lbl_8031554C@ha
    addi r3, r28, 0xc
    addi r4, r4, lbl_8031554C@l
    bl fn_800E01D0
    li r26, 0x0
    mr r25, r26
    b @L_8013BBF4
    @L_8013BBD8:
    mulli r0, r25, 0xc
    addi r4, r28, 0xc
    add r3, r28, r0
    addi r3, r3, 0xc
    bl fn_800E01D0
    add r25, r25, r31
    addi r26, r26, 0x1
    @L_8013BBF4:
    cmpw r26, r30
    blt @L_8013BBD8
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
