    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r5, 0x0
    stw r30, 0x20(r1)
    addi r30, r4, 0x0
    stw r29, 0x1c(r1)
    addi r29, r3, 0x0
    lwz r6, -29632(r13)
    lwz r0, 0x4f4(r6)
    cmplwi r0, 0x0
    beq @L_800B933C
    rlwinm. r0, r0, 0, 31, 31
    beq @L_800B92CC
    bl fn_800BB3C4
@L_800B92CC:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800B92E0
    bl fn_800BC024
@L_800B92E0:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800B92F4
    bl fn_800B9578
@L_800B92F4:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_800B9308
    bl fn_800B7BC4
@L_800B9308:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 27, 27
    beq @L_800B931C
    bl fn_800B8444
@L_800B931C:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 27, 28
    beq @L_800B9330
    bl fn_800B7C18
@L_800B9330:
    lwz r3, -29632(r13)
    li r0, 0x0
    stw r0, 0x4f4(r3)
@L_800B933C:
    lwz r3, -29632(r13)
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800B9350
    bl fn_800B937C
@L_800B9350:
    or r0, r30, r29
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    sth r31, -0x8000(r3)
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
