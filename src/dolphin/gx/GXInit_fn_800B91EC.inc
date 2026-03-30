    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_800B920C
    bl fn_800BB3C4
@L_800B920C:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800B9220
    bl fn_800BC024
@L_800B9220:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800B9234
    bl fn_800B9578
@L_800B9234:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_800B9248
    bl fn_800B7BC4
@L_800B9248:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 27, 27
    beq @L_800B925C
    bl fn_800B8444
@L_800B925C:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    rlwinm. r0, r0, 0, 27, 28
    beq @L_800B9270
    bl fn_800B7C18
@L_800B9270:
    lwz r3, -29632(r13)
    li r0, 0x0
    stw r0, 0x4f4(r3)
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
