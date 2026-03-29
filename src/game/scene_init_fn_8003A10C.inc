    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r31, r3
    stw r31, lbl_8047A4B0(r13)
    @L_8003A124:
    li r3, 0x25
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, -0x1
    beq @L_8003A4F8
    lwz r4, lbl_8047A4A8(r13)
    li r3, 0x0
    lwz r0, lbl_8047A4AC(r13)
    add r30, r4, r0
    bl fn_801347D0
    clrlwi r26, r3, 16
    li r29, -0x1
    li r28, 0x0
    b @L_8003A198
    @L_8003A15C:
    extsh r4, r28
    li r3, 0x0
    bl fn_80134768
    mr r27, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A194
    addi r29, r29, 0x1
    cmpw r29, r30
    blt @L_8003A194
    mr r3, r27
    bl fn_80143C68
    b @L_8003A1A4
    @L_8003A194:
    addi r28, r28, 0x1
    @L_8003A198:
    cmpw r28, r26
    blt @L_8003A15C
    li r3, 0x0
    @L_8003A1A4:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003A4F8
    cmpwi r31, 0x1
    beq @L_8003A360
    bge @L_8003A124
    cmpwi r31, 0x0
    bge @L_8003A1C8
    b @L_8003A124
    @L_8003A1C8:
    li r3, 0x0
    bl fn_801347D0
    clrlwi r26, r3, 16
    li r27, -0x1
    li r28, 0x0
    b @L_8003A220
    @L_8003A1E0:
    extsh r4, r28
    li r3, 0x0
    bl fn_80134768
    mr r29, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A21C
    addi r27, r27, 0x1
    cmpw r27, r30
    blt @L_8003A21C
    mr r3, r29
    bl fn_80143C68
    mr r28, r3
    b @L_8003A22C
    @L_8003A21C:
    addi r28, r28, 0x1
    @L_8003A220:
    cmpw r28, r26
    blt @L_8003A1E0
    li r28, 0x0
    @L_8003A22C:
    li r0, 0x1b6a
    mr r3, r28
    stw r0, lbl_8047A4B4(r13)
    bl fn_801440A0
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x5
    beq @L_8003A2C4
    li r3, 0x0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r27, -0x1
    li r29, 0x0
    b @L_8003A2A0
    @L_8003A264:
    extsh r4, r29
    li r3, 0x0
    bl fn_80134768
    mr r26, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A29C
    addi r27, r27, 0x1
    cmpw r27, r30
    blt @L_8003A29C
    mr r3, r26
    bl fn_80143C50
    b @L_8003A2AC
    @L_8003A29C:
    addi r29, r29, 0x1
    @L_8003A2A0:
    cmpw r29, r25
    blt @L_8003A264
    li r3, 0x0
    @L_8003A2AC:
    clrlwi r5, r3, 16
    li r3, 0x1
    li r4, 0x1
    bl fn_8003ACE8
    mr r27, r3
    b @L_8003A2C8
    @L_8003A2C4:
    li r27, 0x1
    @L_8003A2C8:
    cmpwi r27, 0x0
    ble @L_8003A124
    mr r4, r28
    li r3, 0x0
    bl fn_801298B8
    cmpw r3, r27
    bge @L_8003A304
    li r3, 0x2
    li r4, 0x1b6f
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8003A124
    @L_8003A304:
    mr r4, r28
    clrlwi r5, r27, 16
    li r3, 0x0
    li r6, -0x1
    bl fn_80129A78
    mr r4, r28
    clrlwi r5, r27, 16
    li r3, 0x0
    bl fn_80134584
    clrlwi r4, r28, 16
    li r3, 0x2d
    bl fn_80132A38
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x1b70
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8003A124
    @L_8003A360:
    li r3, 0x0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r26, -0x1
    li r27, 0x0
    b @L_8003A3B8
    @L_8003A378:
    extsh r4, r27
    li r3, 0x0
    bl fn_80134768
    mr r28, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A3B4
    addi r26, r26, 0x1
    cmpw r26, r30
    blt @L_8003A3B4
    mr r3, r28
    bl fn_80143C68
    mr r26, r3
    b @L_8003A3C4
    @L_8003A3B4:
    addi r27, r27, 0x1
    @L_8003A3B8:
    cmpw r27, r25
    blt @L_8003A378
    li r26, 0x0
    @L_8003A3C4:
    mr r3, r26
    bl fn_801440A0
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x5
    bne @L_8003A3FC
    li r3, 0x2
    li r4, 0x1b72
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8003A124
    @L_8003A3FC:
    li r0, 0x1b6b
    li r3, 0x0
    stw r0, lbl_8047A4B4(r13)
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r27, -0x1
    li r28, 0x0
    b @L_8003A458
    @L_8003A41C:
    extsh r4, r28
    li r3, 0x0
    bl fn_80134768
    mr r29, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A454
    addi r27, r27, 0x1
    cmpw r27, r30
    blt @L_8003A454
    mr r3, r29
    bl fn_80143C50
    b @L_8003A464
    @L_8003A454:
    addi r28, r28, 0x1
    @L_8003A458:
    cmpw r28, r25
    blt @L_8003A41C
    li r3, 0x0
    @L_8003A464:
    clrlwi r5, r3, 16
    li r3, 0x1
    li r4, 0x1
    bl fn_8003ACE8
    mr r27, r3
    cmpwi r27, 0x0
    ble @L_8003A124
    clrlwi r25, r26, 16
    li r3, 0x2d
    mr r4, r25
    bl fn_80132A38
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    li r0, 0x1b6c
    stw r0, lbl_8047A4B4(r13)
    bl fn_8003AE84
    cmpwi r3, 0x0
    beq @L_8003A124
    mr r4, r26
    clrlwi r5, r27, 16
    li r3, 0x0
    bl fn_80134584
    mr r4, r25
    li r3, 0x2d
    bl fn_80132A38
    mr r4, r27
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x1b71
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8003A124
    @L_8003A4F8:
    li r3, 0x25
    bl fn_80102510
    li r3, 0x25
    li r4, 0x1
    bl fn_80102428
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
