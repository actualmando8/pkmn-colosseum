    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r30, 0x60(r3)
    bl fn_80105624
    mr r31, r3
    lwz r3, 0x0(r30)
    lhz r0, 0x6(r31)
    lwz r29, 0x0(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    mr r28, r29
    beq @L_8004D828
    bl fn_801D1F7C
    addi r29, r29, 0x1
    cmpw r29, r3
    blt @L_8004D828
    li r29, 0x0
    @L_8004D828:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8004D84C
    subi r29, r29, 0x1
    cmpwi r29, 0x0
    bge @L_8004D84C
    bl fn_801D1F7C
    subi r29, r3, 0x1
    @L_8004D84C:
    cmpw r29, r28
    beq @L_8004D870
    li r3, 0x23
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    lwz r3, 0x0(r30)
    stw r29, 0x0(r3)
    @L_8004D870:
    mr r3, r29
    bl fn_8004BE40
    mr r29, r3
    bl fn_801D1B78
    cmpwi r3, 0x0
    bne @L_8004D898
    mr r3, r29
    bl fn_801D1C20
    clrlwi r3, r29, 16
    bl fn_801D228C
    @L_8004D898:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
