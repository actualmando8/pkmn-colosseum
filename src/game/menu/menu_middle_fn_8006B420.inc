    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r3, 0x8(r3)
    bl fn_80077E50
    cmplwi r3, 0x0
    beq @L_8006B450
    b @L_8006B498
    @L_8006B450:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r31, 0x8(r3)
    cmpwi r31, 0x0
    blt @L_8006B470
    cmplwi r31, 0x6
    blt @L_8006B478
    @L_8006B470:
    li r4, 0x0
    b @L_8006B494
    @L_8006B478:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r31, 0x54
    addis r4, r4, 0x1
    subi r4, r4, 0x3624
    add r4, r3, r4
    @L_8006B494:
    mr r3, r4
    @L_8006B498:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
