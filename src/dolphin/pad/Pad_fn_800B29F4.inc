    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x58(r1)
    stmw r25, 0x3c(r1)
    mr. r26, r4
    addi r25, r3, 0x0
    addi r27, r5, 0x0
    li r30, 0x0
    li r29, 0x0
    li r28, 0x0
    beq @L_800B2A28
    li r0, 0x0
    stw r0, 0x0(r26)
    @L_800B2A28:
    addi r3, r25, 0x0
    addi r4, r1, 0x30
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B2A40
    b @L_800B2F70
    @L_800B2A40:
    lwz r3, 0x30(r1)
    bl fn_800B2220
    mr. r4, r3
    bge @L_800B2A5C
    lwz r3, 0x30(r1)
    bl fn_800B02F4
    b @L_800B2F70
    @L_800B2A5C:
    lwz r3, 0x30(r1)
    addi r4, r1, 0x18
    bl fn_800B24A4
    mr r31, r3
    lwz r3, 0x30(r1)
    addi r4, r1, 0x1c
    bl fn_800B26E4
    add r5, r31, r3
    cmpwi r5, 0x1
    ble @L_800B2A94
    lwz r3, 0x30(r1)
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B2F70
    @L_800B2A94:
    lwz r6, 0x30(r1)
    lwz r4, 0x80(r6)
    addi r3, r4, 0x2000
    addi r0, r4, 0x6000
    stw r3, 0x28(r1)
    addis r3, r4, 0x1
    addi r4, r4, 0x4000
    stw r0, 0x20(r1)
    addi r0, r3, -0x8000
    stw r4, 0x2c(r1)
    stw r0, 0x24(r1)
    beq @L_800B2ACC
    bge @L_800B2B50
    b @L_800B2B50
    @L_800B2ACC:
    addi r3, r6, 0x84
    lwz r0, 0x84(r6)
    cmplwi r0, 0x0
    bne @L_800B2B18
    lwz r0, 0x18(r1)
    addi r4, r1, 0x28
    li r5, 0x2000
    slwi r0, r0, 2
    lwzx r0, r4, r0
    stw r0, 0x0(r3)
    lwz r3, 0x18(r1)
    xori r0, r3, 0x1
    slwi r3, r3, 2
    slwi r0, r0, 2
    lwzx r3, r4, r3
    lwzx r4, r4, r0
    bl memcpy
    li r29, 0x1
    b @L_800B2B50
    @L_800B2B18:
    lwz r0, 0x1c(r1)
    addi r4, r1, 0x20
    li r5, 0x2000
    slwi r0, r0, 2
    lwzx r0, r4, r0
    stw r0, 0x88(r6)
    lwz r3, 0x1c(r1)
    xori r0, r3, 0x1
    slwi r3, r3, 2
    slwi r0, r0, 2
    lwzx r3, r4, r3
    lwzx r4, r4, r0
    bl memcpy
    li r30, 0x1
    @L_800B2B50:
    lwz r0, 0x1c(r1)
    addi r3, r1, 0x20
    li r4, 0x0
    xori r0, r0, 0x1
    slwi r0, r0, 2
    lwzx r31, r3, r0
    li r5, 0x2000
    addi r3, r31, 0x0
    bl memset
    li r0, 0x7f
    lwz r5, 0x30(r1)
    mtctr r0
    li r6, 0x0
    @L_800B2B84:
    lwz r0, 0x84(r5)
    add r7, r0, r6
    lbz r0, 0x0(r7)
    cmplwi r0, 0xff
    beq @L_800B2C3C
    lhz r4, 0x36(r7)
    li r8, 0x0
    b @L_800B2BF4
    @L_800B2BA4:
    clrlwi r3, r4, 16
    cmplwi r3, 0x5
    blt @L_800B2BD8
    lhz r0, 0x10(r5)
    cmplw r3, r0
    bge @L_800B2BD8
    slwi r4, r3, 1
    lhzx r3, r31, r4
    addi r3, r3, 0x1
    clrlwi r0, r3, 16
    sthx r3, r31, r4
    cmplwi r0, 0x1
    ble @L_800B2BE8
    @L_800B2BD8:
    lwz r3, 0x30(r1)
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B2F70
    @L_800B2BE8:
    lwz r3, 0x88(r5)
    addi r8, r8, 0x1
    lhzx r4, r3, r4
    @L_800B2BF4:
    clrlwi r0, r4, 16
    cmplwi r0, 0xffff
    beq @L_800B2C10
    lhz r0, 0x38(r7)
    clrlwi r3, r8, 16
    cmplw r3, r0
    blt @L_800B2BA4
    @L_800B2C10:
    lhz r0, 0x38(r7)
    clrlwi r3, r8, 16
    cmplw r3, r0
    bne @L_800B2C2C
    clrlwi r0, r4, 16
    cmplwi r0, 0xffff
    beq @L_800B2C3C
    @L_800B2C2C:
    lwz r3, 0x30(r1)
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B2F70
    @L_800B2C3C:
    addi r6, r6, 0x40
    bdnz @L_800B2B84
    lwz r3, 0x30(r1)
    addi r6, r31, 0xa
    li r9, 0x0
    li r8, 0x5
    li r5, 0xa
    b @L_800B2CC0
    @L_800B2C5C:
    lwz r4, 0x88(r3)
    lhz r0, 0x0(r6)
    add r4, r4, r5
    cmplwi r0, 0x0
    lhz r0, 0x0(r4)
    bne @L_800B2C90
    cmplwi r0, 0x0
    beq @L_800B2C88
    li r0, 0x0
    sth r0, 0x0(r4)
    li r28, 0x1
    @L_800B2C88:
    addi r9, r9, 0x1
    b @L_800B2CB4
    @L_800B2C90:
    cmplwi r0, 0x5
    blt @L_800B2CA0
    cmplw r0, r7
    blt @L_800B2CB4
    @L_800B2CA0:
    cmplwi r0, 0xffff
    beq @L_800B2CB4
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B2F70
    @L_800B2CB4:
    addi r5, r5, 0x2
    addi r6, r6, 0x2
    addi r8, r8, 0x1
    @L_800B2CC0:
    lhz r7, 0x10(r3)
    clrlwi r0, r8, 16
    cmplw r0, r7
    blt @L_800B2C5C
    lwz r3, 0x88(r3)
    clrlwi r4, r9, 16
    lhzu r0, 0x6(r3)
    cmplw r4, r0
    beq @L_800B2CEC
    sth r9, 0x0(r3)
    li r28, 0x1
    @L_800B2CEC:
    cmpwi r28, 0x0
    beq @L_800B2EB8
    lwz r3, 0x30(r1)
    li r4, 0x1ffc
    srawi r4, r4, 1
    lwz r6, 0x88(r3)
    li r0, 0x0
    addze. r4, r4
    sth r0, 0x2(r6)
    addi r7, r6, 0x2
    addi r5, r6, 0x4
    sth r0, 0x0(r6)
    addi r3, r4, 0x0
    ble @L_800B2E90
    srwi. r0, r3, 3
    mtctr r0
    beq @L_800B2E60
    @L_800B2D30:
    lhz r4, 0x0(r6)
    lhz r0, 0x0(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x0(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0x2(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x2(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0x4(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x4(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0x6(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x6(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0x8(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x8(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0xa(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0xa(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0xc(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0xc(r5)
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    lhz r4, 0x0(r6)
    lhz r0, 0xe(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0xe(r5)
    addi r5, r5, 0x10
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    bdnz @L_800B2D30
    andi. r3, r3, 0x7
    beq @L_800B2E90
    @L_800B2E60:
    mtctr r3
    @L_800B2E64:
    lhz r4, 0x0(r6)
    lhz r0, 0x0(r5)
    add r0, r4, r0
    sth r0, 0x0(r6)
    lhz r0, 0x0(r5)
    addi r5, r5, 0x2
    lhz r4, 0x0(r7)
    nor r0, r0, r0
    add r0, r4, r0
    sth r0, 0x0(r7)
    bdnz @L_800B2E64
    @L_800B2E90:
    lhz r0, 0x0(r6)
    cmplwi r0, 0xffff
    bne @L_800B2EA4
    li r0, 0x0
    sth r0, 0x0(r6)
    @L_800B2EA4:
    lhz r0, 0x0(r7)
    cmplwi r0, 0xffff
    bne @L_800B2EB8
    li r0, 0x0
    sth r0, 0x0(r7)
    @L_800B2EB8:
    lwz r6, 0x1c(r1)
    addi r4, r1, 0x20
    li r5, 0x2000
    xori r0, r6, 0x1
    slwi r3, r0, 2
    slwi r0, r6, 2
    lwzx r3, r4, r3
    lwzx r4, r4, r0
    bl memcpy
    cmpwi r29, 0x0
    beq @L_800B2F04
    cmplwi r26, 0x0
    beq @L_800B2EF4
    li r0, 0x2000
    stw r0, 0x0(r26)
    @L_800B2EF4:
    addi r3, r25, 0x0
    addi r4, r27, 0x0
    bl fn_800B1FAC
    b @L_800B2F70
    @L_800B2F04:
    or. r0, r30, r28
    beq @L_800B2F34
    cmplwi r26, 0x0
    beq @L_800B2F1C
    li r0, 0x2000
    stw r0, 0x0(r26)
    @L_800B2F1C:
    lwz r4, 0x30(r1)
    addi r3, r25, 0x0
    addi r5, r27, 0x0
    lwz r4, 0x88(r4)
    bl fn_800B1D60
    b @L_800B2F70
    @L_800B2F34:
    lwz r3, 0x30(r1)
    li r4, 0x0
    bl fn_800B02F4
    cmplwi r27, 0x0
    beq @L_800B2F6C
    bl OSDisableInterrupts
    addi r12, r27, 0x0
    mtlr r12
    addi r26, r3, 0x0
    addi r3, r25, 0x0
    li r4, 0x0
    blrl
    mr r3, r26
    bl OSRestoreInterrupts
    @L_800B2F6C:
    li r3, 0x0
    @L_800B2F70:
    lmw r25, 0x3c(r1)
    lwz r0, 0x5c(r1)
    addi r1, r1, 0x58
    mtlr r0
    blr
