    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    mr. r28, r3
    beq @L_801B4220
    lwz r0, 0x4(r28)
    cmpwi r0, 0xff
    bne @L_801B3D50
    b @L_801B4220
@L_801B3D50:
    lbz r0, 0x14(r28)
    lwz r30, 0x4(r28)
    cmplwi r0, 0x0
    rlwinm r31, r30, 0, 30, 31
    rlwinm r29, r30, 0, 31, 31
    beq @L_801B3EBC
    lwz r0, 0x18(r28)
    cmpwi r0, 0x0
    bne @L_801B3EBC
    rlwinm r0, r29, 2, 0, 29
    addi r3, r13, -21688
    lwzx r0, r3, r0
    cmpwi r0, 0x0
    beq @L_801B3DC8
    rlwinm r0, r29, 2, 0, 29
    li r5, 0x0
    addi r3, r13, -21688
    addi r4, r1, 0x14
    stwx r5, r3, r0
    addi r3, r29, 0x4
    lwz r0, 0xc(r28)
    stw r0, 0x14(r1)
    bl fn_800BA4C8
    mulli r3, r29, 0x30
    lis r4, -32713
    lwz r0, 0xc(r28)
    addi r4, r4, -12264
    add r3, r4, r3
    stw r0, 0xc(r3)
    b @L_801B3EBC
@L_801B3DC8:
    cmpwi r30, 0x4
    beq @L_801B3DD8
    cmpwi r30, 0x5
    bne @L_801B3E14
@L_801B3DD8:
    mulli r4, r29, 0x30
    lis r3, -32713
    addi r0, r3, -12264
    lwz r3, 0xc(r28)
    add r4, r0, r4
    lwz r0, 0xc(r4)
    cmplw r3, r0
    beq @L_801B3EBC
    mulli r3, r29, 0x30
    lis r4, -32713
    lwz r0, 0xc(r28)
    addi r4, r4, -12264
    add r3, r4, r3
    stw r0, 0xc(r3)
    b @L_801B3EA8
@L_801B3E14:
    cmpwi r30, 0x0
    beq @L_801B3E24
    cmpwi r30, 0x1
    bne @L_801B3E70
@L_801B3E24:
    mulli r4, r29, 0x30
    lis r3, -32713
    addi r0, r3, -12264
    lwz r3, 0xc(r28)
    add r4, r0, r4
    lwz r0, 0xc(r4)
    xor r0, r3, r0
    rlwinm. r0, r0, 0, 0, 23
    beq @L_801B3EBC
    mulli r4, r29, 0x30
    lis r3, -32713
    lwz r0, 0xc(r28)
    addi r3, r3, -12264
    add r3, r3, r4
    addi r4, r3, 0xc
    lwz r3, 0xc(r3)
    rlwimi r3, r0, 0, 0, 23
    stw r3, 0x0(r4)
    b @L_801B3EA8
@L_801B3E70:
    mulli r0, r29, 0x30
    lis r3, -32713
    lbz r4, 0xf(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lbz r0, 0xf(r3)
    cmplw r4, r0
    beq @L_801B3EBC
    mulli r0, r29, 0x30
    lis r3, -32713
    lbz r4, 0xf(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    stb r4, 0xf(r3)
@L_801B3EA8:
    lwz r0, 0xc(r28)
    addi r4, r1, 0x10
    mr r3, r30
    stw r0, 0x10(r1)
    bl fn_800BA4C8
@L_801B3EBC:
    lwz r0, 0x1c(r28)
    cmpwi r0, 0x0
    bne @L_801B4010
    rlwinm r0, r29, 2, 0, 29
    addi r3, r13, -21696
    lwzx r0, r3, r0
    cmpwi r0, 0x0
    beq @L_801B3F1C
    rlwinm r0, r29, 2, 0, 29
    li r5, 0x0
    addi r3, r13, -21696
    addi r4, r1, 0xc
    stwx r5, r3, r0
    addi r3, r29, 0x4
    lwz r0, 0x10(r28)
    stw r0, 0xc(r1)
    bl fn_800BA5BC
    mulli r3, r29, 0x30
    lis r4, -32713
    lwz r0, 0x10(r28)
    addi r4, r4, -12264
    add r3, r4, r3
    stw r0, 0x10(r3)
    b @L_801B4010
@L_801B3F1C:
    cmpwi r30, 0x4
    beq @L_801B3F2C
    cmpwi r30, 0x5
    bne @L_801B3F68
@L_801B3F2C:
    mulli r4, r29, 0x30
    lis r3, -32713
    addi r0, r3, -12264
    lwz r3, 0x10(r28)
    add r4, r0, r4
    lwz r0, 0x10(r4)
    cmplw r3, r0
    beq @L_801B4010
    mulli r3, r29, 0x30
    lis r4, -32713
    lwz r0, 0x10(r28)
    addi r4, r4, -12264
    add r3, r4, r3
    stw r0, 0x10(r3)
    b @L_801B3FFC
@L_801B3F68:
    cmpwi r30, 0x0
    beq @L_801B3F78
    cmpwi r30, 0x1
    bne @L_801B3FC4
@L_801B3F78:
    mulli r4, r29, 0x30
    lis r3, -32713
    addi r0, r3, -12264
    lwz r3, 0x10(r28)
    add r4, r0, r4
    lwz r0, 0x10(r4)
    xor r0, r3, r0
    rlwinm. r0, r0, 0, 0, 23
    beq @L_801B4010
    mulli r4, r29, 0x30
    lis r3, -32713
    lwz r0, 0x10(r28)
    addi r3, r3, -12264
    add r3, r3, r4
    addi r4, r3, 0x10
    lwz r3, 0x10(r3)
    rlwimi r3, r0, 0, 0, 23
    stw r3, 0x0(r4)
    b @L_801B3FFC
@L_801B3FC4:
    mulli r0, r29, 0x30
    lis r3, -32713
    lbz r4, 0x13(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lbz r0, 0x13(r3)
    cmplw r4, r0
    beq @L_801B4010
    mulli r0, r29, 0x30
    lis r3, -32713
    lbz r4, 0x13(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    stb r4, 0x13(r3)
@L_801B3FFC:
    lwz r0, 0x10(r28)
    addi r4, r1, 0x8
    mr r3, r30
    stw r0, 0x8(r1)
    bl fn_800BA5BC
@L_801B4010:
    mulli r0, r31, 0x30
    lis r3, -32713
    lbz r4, 0x14(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lbz r0, 0x14(r3)
    cmplw r4, r0
    bne @L_801B40D0
    mulli r0, r31, 0x30
    lis r3, -32713
    lwz r4, 0x18(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lwz r0, 0x18(r3)
    cmpw r4, r0
    bne @L_801B40D0
    mulli r0, r31, 0x30
    lis r3, -32713
    lwz r4, 0x1c(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    cmpw r4, r0
    bne @L_801B40D0
    mulli r0, r31, 0x30
    lis r3, -32713
    lwz r4, 0x20(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lwz r0, 0x20(r3)
    cmpw r4, r0
    bne @L_801B40D0
    mulli r0, r31, 0x30
    lis r3, -32713
    lwz r4, 0x24(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lwz r0, 0x24(r3)
    cmpw r4, r0
    bne @L_801B40D0
    mulli r0, r31, 0x30
    lis r3, -32713
    lwz r4, 0x28(r28)
    addi r3, r3, -12264
    add r3, r3, r0
    lwz r0, 0x28(r3)
    cmpw r4, r0
    beq @L_801B4220
@L_801B40D0:
    lbz r4, 0x14(r28)
    mr r3, r30
    lwz r5, 0x18(r28)
    lwz r6, 0x1c(r28)
    lwz r7, 0x20(r28)
    lwz r8, 0x24(r28)
    lwz r9, 0x28(r28)
    bl fn_800BA6F4
    mulli r0, r31, 0x30
    lis r4, -32713
    lis r3, -32713
    lbz r5, 0x14(r28)
    addi r4, r4, -12264
    add r4, r4, r0
    stb r5, 0x14(r4)
    mulli r0, r31, 0x30
    addi r4, r3, -12264
    lwz r6, 0x18(r28)
    lis r3, -32713
    add r4, r4, r0
    lis r5, -32713
    stw r6, 0x18(r4)
    mulli r0, r31, 0x30
    addi r3, r3, -12264
    lwz r7, 0x1c(r28)
    lis r4, -32713
    add r6, r3, r0
    lis r3, -32713
    stw r7, 0x1c(r6)
    mulli r0, r31, 0x30
    addi r5, r5, -12264
    lwz r6, 0x20(r28)
    addi r4, r4, -12264
    add r5, r5, r0
    addi r3, r3, -12264
    stw r6, 0x20(r5)
    mulli r0, r31, 0x30
    cmpwi r30, 0x4
    lwz r5, 0x24(r28)
    add r4, r4, r0
    stw r5, 0x24(r4)
    mulli r0, r31, 0x30
    lwz r4, 0x28(r28)
    add r3, r3, r0
    stw r4, 0x28(r3)
    beq @L_801B4190
    cmpwi r30, 0x5
    bne @L_801B4220
@L_801B4190:
    mulli r0, r31, 0x30
    lis r4, -32713
    lis r3, -32713
    lbz r5, 0x14(r28)
    addi r4, r4, -12264
    add r4, r4, r0
    stb r5, 0x74(r4)
    mulli r0, r31, 0x30
    addi r4, r3, -12264
    lwz r6, 0x18(r28)
    lis r3, -32713
    add r4, r4, r0
    lis r5, -32713
    stw r6, 0x78(r4)
    mulli r0, r31, 0x30
    addi r3, r3, -12264
    lwz r7, 0x1c(r28)
    lis r4, -32713
    add r6, r3, r0
    lis r3, -32713
    stw r7, 0x7c(r6)
    mulli r0, r31, 0x30
    addi r5, r5, -12264
    lwz r6, 0x20(r28)
    addi r4, r4, -12264
    add r5, r5, r0
    addi r3, r3, -12264
    stw r6, 0x80(r5)
    mulli r0, r31, 0x30
    lwz r5, 0x24(r28)
    add r4, r4, r0
    stw r5, 0x84(r4)
    mulli r0, r31, 0x30
    lwz r4, 0x28(r28)
    add r3, r3, r0
    stw r4, 0x88(r3)
@L_801B4220:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
