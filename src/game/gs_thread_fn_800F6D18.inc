    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r7, 0x0
    lis r6, lbl_80271068@ha
    stw r0, 0x24(r1)
    mr r0, r7
    li r8, 0x0
    stmw r26, 0x8(r1)
    mr r27, r4
    mr r26, r3
    mr r28, r5
    mr r4, r7
    addi r31, r6, lbl_80271068@l
    b @L_800F6D80
    @L_800F6D50:
    lwz r3, 0xc(r5)
    add r5, r3, r7
    lbz r3, 0x4(r5)
    cmplwi r3, 0x4
    bne @L_800F6D68
    stb r4, 0x4(r5)
    @L_800F6D68:
    lbz r3, 0x4(r5)
    cmplwi r3, 0x3
    bne @L_800F6D78
    stb r0, 0x4(r5)
    @L_800F6D78:
    addi r7, r7, 0x16c
    addi r8, r8, 0x1
    @L_800F6D80:
    lwz r5, lbl_80478B00(r13)
    lhz r6, 0x0(r5)
    cmplw r8, r6
    blt @L_800F6D50
    li r4, 0x0
    lwz r30, 0xc(r5)
    mr r3, r4
    mtctr r6
    cmplwi r6, 0x0
    ble @L_800F6DC8
    @L_800F6DA8:
    lwz r0, 0xc(r5)
    add r30, r0, r3
    lbz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_800F6DC8
    addi r3, r3, 0x16c
    addi r4, r4, 0x1
    bdnz @L_800F6DA8
    @L_800F6DC8:
    cmplw r4, r6
    bne @L_800F6DE8
    mr r4, r26
    addi r3, r31, 0x178
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, 0x0
    b @L_800F7054
    @L_800F6DE8:
    mr r3, r30
    li r4, 0x0
    li r5, 0x16c
    bl memset
    lwz r3, lbl_80478B00(r13)
    srwi r4, r26, 16
    lwz r3, 0x8(r3)
    b @L_800F6E2C
    @L_800F6E08:
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800F6E28
    stw r3, 0x0(r30)
    lwz r0, 0x10(r3)
    add r0, r3, r0
    stw r0, 0x18(r30)
    b @L_800F6E34
    @L_800F6E28:
    lwz r3, 0x14(r3)
    @L_800F6E2C:
    cmplwi r3, 0x0
    bne @L_800F6E08
    @L_800F6E34:
    cmplwi r3, 0x0
    bne @L_800F6E54
    mr r4, r26
    addi r3, r31, 0x1b4
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, 0x0
    b @L_800F7054
    @L_800F6E54:
    lwz r7, lbl_80478B00(r13)
    lhz r6, 0x4(r7)
    mr r29, r6
    @L_800F6E60:
    addi r0, r29, 0x1
    clrlwi r0, r0, 16
    cmplw r0, r6
    mr r29, r0
    bne @L_800F6E8C
    mr r4, r26
    addi r3, r31, 0x1f0
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, 0x0
    b @L_800F7054
    @L_800F6E8C:
    cmplwi r0, 0x0
    beq @L_800F6E60
    lhz r3, 0x0(r7)
    clrlwi r0, r0, 16
    li r5, 0x0
    mtctr r3
    cmpwi r3, 0x0
    ble @L_800F6ED8
    @L_800F6EAC:
    lwz r3, 0xc(r7)
    add r4, r3, r5
    lbz r3, 0x4(r4)
    cmplwi r3, 0x0
    beq @L_800F6ED0
    lhz r3, 0x6(r4)
    cmplw r3, r0
    bne @L_800F6ED0
    b @L_800F6EDC
    @L_800F6ED0:
    addi r5, r5, 0x16c
    bdnz @L_800F6EAC
    @L_800F6ED8:
    li r4, 0x0
    @L_800F6EDC:
    cmplwi r4, 0x0
    bne @L_800F6E60
    sth r29, 0x4(r7)
    clrlwi r4, r26, 16
    lwz r5, 0x0(r30)
    lhz r0, 0x4(r5)
    cmplw r4, r0
    blt @L_800F6F0C
    addi r3, r31, 0xf4
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6F20
    @L_800F6F0C:
    slwi r0, r4, 2
    add r3, r5, r0
    lwz r0, 0x18(r3)
    add r0, r5, r0
    stw r0, 0x14(r30)
    @L_800F6F20:
    li r0, 0x1
    stb r0, 0x4(r30)
    stw r26, 0x8(r30)
    sth r29, 0x6(r30)
    lwz r4, 0x28(r30)
    cmpwi r4, 0x40
    ble @L_800F6F4C
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6F64
    @L_800F6F4C:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r30)
    add r3, r30, r0
    li r0, 0x0
    stw r0, 0x6c(r3)
    @L_800F6F64:
    li r26, 0x0
    b @L_800F6FB0
    @L_800F6F6C:
    mr r3, r28
    li r4, 0x1
    bl __va_arg
    lwz r4, 0x28(r30)
    lwz r5, 0x0(r3)
    cmpwi r4, 0x40
    ble @L_800F6F98
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6FAC
    @L_800F6F98:
    addi r0, r4, 0x1
    slwi r3, r4, 2
    stw r0, 0x28(r30)
    addi r0, r3, 0x6c
    stwx r5, r30, r0
    @L_800F6FAC:
    addi r26, r26, 0x1
    @L_800F6FB0:
    cmplw r26, r27
    blt @L_800F6F6C
    lwz r4, 0x28(r30)
    cmpwi r4, 0x40
    ble @L_800F6FD4
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6FEC
    @L_800F6FD4:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r30)
    add r3, r30, r0
    li r0, 0x0
    stw r0, 0x6c(r3)
    @L_800F6FEC:
    lwz r4, 0x28(r30)
    cmpwi r4, 0x40
    ble @L_800F7008
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F7020
    @L_800F7008:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r30)
    add r3, r30, r0
    li r0, 0x0
    stw r0, 0x6c(r3)
    @L_800F7020:
    lwz r4, 0x28(r30)
    cmpwi r4, 0x40
    ble @L_800F703C
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F7050
    @L_800F703C:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r30)
    add r3, r30, r0
    stw r27, 0x6c(r3)
    @L_800F7050:
    mr r3, r30
    @L_800F7054:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
