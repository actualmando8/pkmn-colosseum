    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lis r3, -32709
    addi r31, r3, 28040
    lwz r0, 0x40(r31)
    rlwinm r0, r0, 3, 0, 28
    lwzx r30, r31, r0
    bl fn_801046B8
    cmpw r3, r30
    bne @L_8007151C
    lwz r0, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    li r4, 0x0
    rlwinm r0, r0, 3, 0, 28
    li r5, 0x0
    lwzx r3, r3, r0
    bl fn_80102568
@L_8007151C:
    li r3, 0xbe
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8007153C
    li r3, 0xbe
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
@L_8007153C:
    lwz r0, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    li r4, 0x0
    rlwinm r0, r0, 3, 0, 28
    add r3, r3, r0
    stw r4, 0x4(r3)
    lwz r0, 0x40(r31)
    cmpwi r0, 0x0
    bne @L_8007156C
    li r3, -0x1
    b @L_800715A4
@L_8007156C:
    bgt @L_80071588
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30968
    li r4, 0x5c
    addi r5, r5, -30952
    bl fn_80196E10
@L_80071588:
    lwz r4, 0x40(r31)
    lis r3, -32709
    addi r3, r3, 28040
    subi r0, r4, 0x1
    stw r0, 0x40(r31)
    rlwinm r0, r0, 3, 0, 28
    lwzx r3, r3, r0
@L_800715A4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
