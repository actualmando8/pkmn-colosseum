    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r27, r4
    mr r30, r3
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_802230E8
    li r3, 0x1
    b @L_802232E0
@L_802230E8:
    mr r3, r30
    bl fn_801FA634
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80223100
    li r3, 0x1
    b @L_802232E0
@L_80223100:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    li r31, 0x0
    b @L_802232D0
@L_80223120:
    mr r3, r30
    mr r6, r31
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r29, r3
    beq @L_802232CC
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802232CC
    mr r3, r29
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802232CC
    li r0, 0x0
    mr r3, r30
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    bl fn_801F8A18
    cmplwi r3, 0x0
    beq @L_802232DC
    mr r3, r29
    li r24, -0x1
    li r25, 0x0
    bl fn_80206780
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802231F4
    mr r3, r30
    mr r4, r29
    bl fn_801F85B0
    mr r0, r3
    mr r3, r30
    rlwinm r6, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r26, r3
    bl fn_80206608
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802231F4
    mr r25, r26
    mr r3, r26
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r24, r3
@L_802231F4:
    cmplwi r25, 0x0
    bne @L_80223240
    mr r3, r30
    mr r4, r27
    mr r6, r29
    li r5, 0x0
    bl fn_801F9600
    mr r24, r3
    extsh. r0, r24
    bge @L_80223234
    extsh r0, r24
    cmpwi r0, -0x2
    bne @L_802232DC
    li r3, 0x0
    bl fn_801F150C
    b @L_802232DC
@L_80223234:
    mr r3, r30
    mr r4, r24
    bl fn_801F8F24
@L_80223240:
    mr r3, r29
    bl fn_80206780
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022329C
    mr r3, r30
    mr r4, r29
    bl fn_801F85B0
    mr r0, r3
    mr r3, r30
    rlwinm r6, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r4, r3
    mr r3, r29
    li r5, 0x0
    bl fn_802068C8
    mr r3, r29
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_8022329C:
    mr r3, r29
    extsh r7, r24
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_802232CC:
    addi r31, r31, 0x1
@L_802232D0:
    rlwinm r0, r31, 0, 16, 31
    cmplw r0, r28
    blt @L_80223120
@L_802232DC:
    li r3, 0x1
@L_802232E0:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
