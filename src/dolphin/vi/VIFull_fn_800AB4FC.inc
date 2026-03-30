    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    mr r27, r3
    bl OSDisableInterrupts
    addi r30, r3, 0x0
    li r28, 0x0
    li r29, 0x0
    lis r31, 0x8000
    @L_800AB524:
    lwz r3, lbl_8047A8A4(r13)
    srw r0, r31, r29
    and. r0, r3, r0
    beq @L_800AB57C
    mr r3, r29
    bl SIGetType
    rlwinm. r0, r3, 0, 2, 2
    bne @L_800AB57C
    lwz r0, lbl_80478A18(r13)
    lwz r3, 0x0(r27)
    cmplwi r0, 0x2
    bge @L_800AB560
    cmplwi r3, 0x2
    bne @L_800AB560
    li r3, 0x0
    @L_800AB560:
    lwz r4, lbl_80478A14(r13)
    clrlwi r0, r3, 30
    addi r3, r29, 0x0
    oris r4, r4, 0x40
    or r4, r4, r0
    bl fn_800D0338
    li r28, 0x1
    @L_800AB57C:
    addi r29, r29, 0x1
    cmpwi r29, 0x4
    addi r27, r27, 0x4
    blt @L_800AB524
    cmpwi r28, 0x0
    beq @L_800AB598
    bl fn_800D034C
    @L_800AB598:
    mr r3, r30
    bl OSRestoreInterrupts
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
