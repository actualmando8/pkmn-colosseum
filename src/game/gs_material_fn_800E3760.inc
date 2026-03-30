    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x0(r3)
    clrlwi. r0, r0, 31
    beq @L_800E3868
    li r3, 0x7f
    bl fn_801B25C4
    bl fn_800D2584
    mr. r31, r3
    beq @L_800E3860
    lwz r3, 0xc(r31)
    bl fn_80195A6C
    cmpwi r3, 0x0
    beq @L_800E384C
    lwz r3, 0xc(r31)
    bl fn_800DD174
    mr r3, r29
    li r4, 0x1
    bl fn_800E9148
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E37D8
    lwz r31, 0xc(r29)
    b @L_800E37DC
    @L_800E37D8:
    lwz r31, 0x8(r29)
    @L_800E37DC:
    rlwinm. r0, r30, 0, 27, 27
    beq @L_800E3804
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_801A13CC
    lwz r3, 0x168(r29)
    lwz r4, 0x16c(r29)
    bl fn_800D6A5C
    @L_800E3804:
    rlwinm. r0, r30, 0, 19, 19
    beq @L_800E3820
    mr r3, r31
    li r4, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801A13CC
    @L_800E3820:
    rlwinm. r0, r30, 0, 18, 18
    beq @L_800E383C
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801A13CC
    @L_800E383C:
    mr r3, r29
    li r4, 0x0
    bl fn_800E9148
    bl fn_80195A48
    @L_800E384C:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 10, 10
    beq @L_800E3860
    addi r3, r29, 0x4c
    bl fn_80190E60
    @L_800E3860:
    li r3, -0x1
    bl fn_800D87AC
    @L_800E3868:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
