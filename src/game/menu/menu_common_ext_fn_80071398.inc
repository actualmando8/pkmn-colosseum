    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    lis r3, -32709
    addi r31, r3, 28040
    lwz r0, 0x40(r31)
    rlwinm r0, r0, 3, 0, 28
    lwzx r29, r31, r0
    bl fn_801046B8
    cmpw r3, r29
    bne @L_800713F4
    lwz r0, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    li r4, 0x0
    rlwinm r0, r0, 3, 0, 28
    li r5, 0x0
    lwzx r3, r3, r0
    bl fn_80102568
@L_800713F4:
    li r3, 0xbe
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80071414
    li r3, 0xbe
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
@L_80071414:
    lwz r0, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    li r4, 0x0
    rlwinm r0, r0, 3, 0, 28
    add r3, r3, r0
    stw r4, 0x4(r3)
    lwz r0, 0x40(r31)
    cmpwi r0, 0x0
    beq @L_80071464
    bgt @L_80071458
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30968
    li r4, 0x5c
    addi r5, r5, -30952
    bl fn_80196E10
@L_80071458:
    lwz r3, 0x40(r31)
    subi r0, r3, 0x1
    stw r0, 0x40(r31)
@L_80071464:
    lis r3, -32709
    addi r3, r3, 28040
    b @L_80071484
@L_80071470:
    cmpwi r4, 0x0
    beq @L_80071498
    lwz r4, 0x40(r31)
    subi r0, r4, 0x1
    stw r0, 0x40(r31)
@L_80071484:
    lwz r4, 0x40(r31)
    rlwinm r0, r4, 3, 0, 28
    lwzx r0, r3, r0
    cmpw r30, r0
    bne @L_80071470
@L_80071498:
    lwz r0, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    rlwinm r0, r0, 3, 0, 28
    lwzx r3, r3, r0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
