    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    bl fn_80105624
    mr r31, r3
    lwz r30, 0x60(r25)
    lhz r3, 0x4(r31)
    rlwinm r0, r3, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_80027DB4
    lwz r4, 0x24(r30)
    lwz r3, 0x0(r4)
    addi r3, r3, 0x1
    cmpwi r3, 0x2
    blt @L_80027DA0
    li r3, 0x0
    @L_80027DA0:
    stw r3, 0x0(r4)
    li r3, 0x27
    bl fn_80166A28
    li r3, 0x0
    b @L_800280E8
    @L_80027DB4:
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80027E40
    mr r3, r30
    bl fn_80027AA4
    cmpwi r3, 0x0
    beq @L_80027DE0
    li r0, 0x1
    li r3, 0x0
    stb r0, 0x98(r25)
    b @L_800280E8
    @L_80027DE0:
    lwz r3, 0x18(r30)
    li r4, 0x0
    b @L_80027DF4
    @L_80027DEC:
    addi r3, r3, 0x2
    addi r4, r4, 0x1
    @L_80027DF4:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_80027DEC
    lwz r0, 0x1c(r30)
    lis r3, lbl_80266DD8@ha
    addi r3, r3, lbl_80266DD8@l
    slwi r0, r0, 4
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpw r4, r0
    blt @L_80027E38
    lwz r3, 0x28(r30)
    li r4, 0xf
    li r0, 0x3
    stw r4, 0x0(r3)
    lwz r3, 0x2c(r30)
    stw r0, 0x0(r3)
    @L_80027E38:
    li r3, 0x0
    b @L_800280E8
    @L_80027E40:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80027EA0
    lwz r3, 0x34(r30)
    lwz r3, 0x0(r3)
    cmpwi r3, 0x0
    bgt @L_80027E64
    li r0, 0x0
    b @L_80027E84
    @L_80027E64:
    subi r4, r3, 0x1
    lwz r5, 0x18(r30)
    slwi r3, r4, 1
    li r0, 0x0
    sthx r0, r5, r3
    li r0, 0x1
    lwz r3, 0x34(r30)
    stw r4, 0x0(r3)
    @L_80027E84:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_80027E98
    li r3, 0x25
    bl fn_80166A28
    @L_80027E98:
    li r3, 0x0
    b @L_800280E8
    @L_80027EA0:
    rlwinm r0, r3, 0, 21, 21
    cmpwi r0, 0x0
    beq @L_80027F88
    lwz r3, 0x34(r30)
    lwz r3, 0x0(r3)
    subi r0, r3, 0x1
    cmpwi r0, 0x0
    blt @L_80027F78
    lwz r28, 0x18(r30)
    slwi r29, r0, 1
    li r27, 0x0
    lhzx r26, r28, r29
    mr r30, r27
    @L_80027ED4:
    la r25, lbl_8047B920(r2)
    lwzx r3, r25, r30
    cmplwi r3, 0x0
    beq @L_80027F2C
    bl fn_800FA314
    mr r31, r3
    lwzx r3, r25, r30
    bl fn_800FA280
    addi r3, r3, 0x2
    li r4, 0x1
    srwi r0, r31, 1
    mtctr r0
    cmpwi r31, 0x1
    ble @L_80027F24
    @L_80027F0C:
    lhz r0, 0x0(r3)
    cmplw r0, r26
    beq @L_80027F24
    addi r4, r4, 0x2
    addi r3, r3, 0x4
    bdnz @L_80027F0C
    @L_80027F24:
    cmpw r4, r31
    blt @L_80027F3C
    @L_80027F2C:
    addi r27, r27, 0x1
    addi r30, r30, 0x4
    cmpwi r27, 0x2
    blt @L_80027ED4
    @L_80027F3C:
    cmpwi r27, 0x2
    blt @L_80027F48
    li r27, 0x0
    @L_80027F48:
    mr r25, r27
    @L_80027F4C:
    addi r25, r25, 0x1
    cmpwi r25, 0x2
    blt @L_80027F5C
    li r25, 0x0
    @L_80027F5C:
    lhzx r3, r28, r29
    mr r4, r25
    bl fn_80027960
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80027F4C
    sthx r3, r28, r29
    @L_80027F78:
    li r3, 0x24
    bl fn_80166A28
    li r3, 0x0
    b @L_800280E8
    @L_80027F88:
    rlwinm r0, r3, 0, 20, 20
    cmpwi r0, 0x0
    beq @L_80027FA4
    li r0, 0x1
    li r3, 0x0
    stb r0, 0x98(r25)
    b @L_800280E8
    @L_80027FA4:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80027FE0
    lwz r3, 0x28(r30)
    lwz r3, 0x0(r3)
    addi r25, r3, 0x1
    cmpwi r25, 0x10
    blt @L_80027FD0
    li r25, 0xf
    b @L_80027FD8
    @L_80027FD0:
    li r3, 0x23
    bl fn_80166A28
    @L_80027FD8:
    lwz r3, 0x28(r30)
    stw r25, 0x0(r3)
    @L_80027FE0:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8002801C
    lwz r3, 0x28(r30)
    lwz r3, 0x0(r3)
    subi r25, r3, 0x1
    cmpwi r25, 0x0
    bge @L_8002800C
    li r25, 0x0
    b @L_80028014
    @L_8002800C:
    li r3, 0x23
    bl fn_80166A28
    @L_80028014:
    lwz r3, 0x28(r30)
    stw r25, 0x0(r3)
    @L_8002801C:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80028080
    lwz r3, 0x28(r30)
    lwz r4, 0x2c(r30)
    lwz r0, 0x0(r3)
    lwz r25, 0x0(r4)
    cmpwi r0, 0xf
    blt @L_8002805C
    cmpwi r25, 0x1
    bne @L_80028054
    addi r25, r25, 0x2
    b @L_80028060
    @L_80028054:
    addi r25, r25, 0x1
    b @L_80028060
    @L_8002805C:
    addi r25, r25, 0x1
    @L_80028060:
    cmpwi r25, 0x4
    blt @L_80028070
    li r25, 0x3
    b @L_80028078
    @L_80028070:
    li r3, 0x23
    bl fn_80166A28
    @L_80028078:
    lwz r3, 0x2c(r30)
    stw r25, 0x0(r3)
    @L_80028080:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_800280E4
    lwz r3, 0x28(r30)
    lwz r4, 0x2c(r30)
    lwz r0, 0x0(r3)
    lwz r25, 0x0(r4)
    cmpwi r0, 0xf
    blt @L_800280C0
    cmpwi r25, 0x2
    bne @L_800280B8
    subi r25, r25, 0x2
    b @L_800280C4
    @L_800280B8:
    subi r25, r25, 0x1
    b @L_800280C4
    @L_800280C0:
    subi r25, r25, 0x1
    @L_800280C4:
    cmpwi r25, 0x0
    bge @L_800280D4
    li r25, 0x0
    b @L_800280DC
    @L_800280D4:
    li r3, 0x23
    bl fn_80166A28
    @L_800280DC:
    lwz r3, 0x2c(r30)
    stw r25, 0x0(r3)
    @L_800280E4:
    li r3, 0x0
    @L_800280E8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
