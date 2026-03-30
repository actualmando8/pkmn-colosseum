    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    rlwinm r29, r26, 0, 16, 31
    mr r3, r29
    bl fn_801D1504
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D22C4
    cmplwi r3, 0x0
    beq @L_801D22C4
    bl fn_80190528
@L_801D22C4:
    mr r3, r29
    bl fn_801D139C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xffff
    beq @L_801D22DC
    bl fn_801D1364
@L_801D22DC:
    mr r3, r29
    bl fn_801D19A4
    rlwinm r30, r3, 0, 16, 31
    cmplwi r30, 0xffff
    beq @L_801D2354
    li r27, 0x0
    b @L_801D2348
@L_801D22F8:
    rlwinm r31, r27, 0, 16, 31
    mr r3, r29
    mr r4, r31
    bl fn_801D1864
    li r4, 0x1
    mr r28, r3
    bl fn_801EED30
    mr r3, r29
    mr r4, r31
    bl fn_801D1734
    mr r31, r3
    mr r3, r28
    bl fn_801EE614
    rlwinm r0, r3, 0, 16, 31
    rlwinm r4, r31, 0, 24, 31
    cmpw r0, r4
    bge @L_801D2344
    mr r3, r28
    bl fn_801EE67C
@L_801D2344:
    addi r27, r27, 0x1
@L_801D2348:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r30
    blt @L_801D22F8
@L_801D2354:
    mr r3, r29
    bl fn_801D14C0
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D237C
    cmplwi r3, 0x0
    beq @L_801D237C
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800F7434
@L_801D237C:
    mr r3, r29
    bl fn_801D147C
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D23AC
    cmplwi r3, 0x0
    beq @L_801D23AC
    rlwinm r4, r26, 0, 16, 31
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8012BAD0
@L_801D23AC:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
