    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr. r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    beq @L_801A1A74
    cmplwi r29, 0x0
    bne @L_801A1A44
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1A44:
    lwz r0, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1A64
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1A64
    li r3, 0x1
@L_801A1A64:
    cmpwi r3, 0x0
    beq @L_801A1A74
    mr r3, r29
    bl fn_8019D9DC
@L_801A1A74:
    lwz r31, 0x10(r29)
    cmplwi r31, 0x0
    beq @L_801A1AC8
    cmplwi r31, 0x0
    bne @L_801A1A98
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1A98:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1AB8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1AB8
    li r3, 0x1
@L_801A1AB8:
    cmpwi r3, 0x0
    beq @L_801A1AC8
    mr r3, r31
    bl fn_8019D9DC
@L_801A1AC8:
    lwz r3, 0x10(r29)
    mr r4, r30
    addi r3, r3, 0x44
    bl fn_800A2EB4
    mr r4, r30
    mr r5, r30
    addi r3, r29, 0x44
    bl fn_800A2D98
    cmplwi r28, 0x0
    beq @L_801A1B04
    mr r3, r28
    mr r4, r30
    mr r5, r30
    bl fn_800A2D98
    b @L_801A1B20
@L_801A1B04:
    bl fn_801942B8
    cmplwi r3, 0x0
    beq @L_801A1B20
    addi r3, r3, 0x54
    mr r4, r30
    mr r5, r30
    bl fn_800A2D98
@L_801A1B20:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
