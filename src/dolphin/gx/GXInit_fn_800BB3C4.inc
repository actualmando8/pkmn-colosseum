    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    lwz r3, -29632(r13)
    lwz r0, 0x4dc(r3)
    cmplwi r0, 0xff
    beq @L_800BB52C
    lwz r0, 0x204(r3)
    li r31, 0x0
    rlwinm r3, r0, 22, 28, 31
    addi r30, r3, 0x1
    rlwinm r27, r0, 16, 29, 31
    b @L_800BB498
@L_800BB3FC:
    cmpwi r31, 0x2
    beq @L_800BB44C
    bge @L_800BB418
    cmpwi r31, 0x0
    beq @L_800BB424
    bge @L_800BB438
    b @L_800BB470
@L_800BB418:
    cmpwi r31, 0x4
    bge @L_800BB470
    b @L_800BB460
@L_800BB424:
    lwz r3, -29632(r13)
    lwz r0, 0x120(r3)
    rlwinm r29, r0, 0, 29, 31
    rlwinm r28, r0, 29, 29, 31
    b @L_800BB470
@L_800BB438:
    lwz r3, -29632(r13)
    lwz r0, 0x120(r3)
    rlwinm r29, r0, 26, 29, 31
    rlwinm r28, r0, 23, 29, 31
    b @L_800BB470
@L_800BB44C:
    lwz r3, -29632(r13)
    lwz r0, 0x120(r3)
    rlwinm r29, r0, 20, 29, 31
    rlwinm r28, r0, 17, 29, 31
    b @L_800BB470
@L_800BB460:
    lwz r3, -29632(r13)
    lwz r0, 0x120(r3)
    rlwinm r29, r0, 14, 29, 31
    rlwinm r28, r0, 11, 29, 31
@L_800BB470:
    lwz r3, -29632(r13)
    li r0, 0x1
    slw r0, r0, r28
    lwz r3, 0x4dc(r3)
    and. r0, r3, r0
    bne @L_800BB494
    addi r3, r29, 0x0
    addi r4, r28, 0x0
    bl fn_800BB30C
@L_800BB494:
    addi r31, r31, 0x1
@L_800BB498:
    cmplw r31, r27
    blt @L_800BB3FC
    li r31, 0x0
    addi r27, r31, 0x0
    b @L_800BB524
@L_800BB4AC:
    lwz r5, -29632(r13)
    addi r3, r27, 0x49c
    rlwinm r4, r31, 1, 0, 29
    lwzx r3, r5, r3
    rlwinm. r0, r31, 0, 31, 31
    addi r4, r4, 0x100
    add r4, r5, r4
    rlwinm r29, r3, 0, 24, 22
    beq @L_800BB4DC
    lwz r0, 0x0(r4)
    rlwinm r28, r0, 17, 29, 31
    b @L_800BB4E4
@L_800BB4DC:
    lwz r0, 0x0(r4)
    rlwinm r28, r0, 29, 29, 31
@L_800BB4E4:
    cmplwi r29, 0xff
    beq @L_800BB51C
    li r4, 0x1
    lwz r3, 0x4dc(r5)
    slw r0, r4, r28
    and. r0, r3, r0
    bne @L_800BB51C
    lwz r3, 0x4e0(r5)
    slw r0, r4, r31
    and. r0, r3, r0
    beq @L_800BB51C
    addi r3, r29, 0x0
    addi r4, r28, 0x0
    bl fn_800BB30C
@L_800BB51C:
    addi r27, r27, 0x4
    addi r31, r31, 0x1
@L_800BB524:
    cmplw r31, r30
    blt @L_800BB4AC
@L_800BB52C:
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
