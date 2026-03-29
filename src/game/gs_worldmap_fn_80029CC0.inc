    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stmw r24, 0xe0(r1)
    mr r30, r3
    li r4, 0xeb
    bl fn_80142A88
    addi r3, r30, 0x3ac
    li r4, 0xeb
    bl fn_80142A88
    li r0, 0x0
    addi r6, r1, 0x8
    stw r0, 0x758(r30)
    li r3, 0x0
    li r4, 0x40
    li r5, 0x0
    stw r0, 0x75c(r30)
    bl fn_800849B4
    cmpwi r3, 0x0
    bge @L_80029D18
    li r3, 0x0
    b @L_80029EE0
    @L_80029D18:
    addi r27, r1, 0x8
    li r31, 0x0
    b @L_80029EA8
    @L_80029D24:
    lhz r26, 0x10(r27)
    cmplwi r26, 0x0
    beq @L_80029EA0
    extsh r0, r31
    lhz r28, 0x12(r27)
    cmpwi r0, -0x1
    blt @L_80029EA0
    cmpw r0, r25
    bge @L_80029EA0
    cmpwi r0, -0x1
    beq @L_80029DDC
    cmpwi r0, 0x0
    blt @L_80029EA0
    cmpw r0, r25
    bge @L_80029EA0
    slwi r0, r0, 2
    add r25, r30, r0
    mr r3, r25
    bl fn_80143C68
    clrlwi r0, r3, 16
    cmplw r0, r26
    beq @L_80029D84
    cmplwi r0, 0x0
    bne @L_80029EA0
    @L_80029D84:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    bne @L_80029DA4
    mr r3, r25
    mr r4, r26
    bl fn_80143B80
    li r3, 0x0
    b @L_80029DAC
    @L_80029DA4:
    mr r3, r25
    bl fn_80143C50
    @L_80029DAC:
    clrlwi r4, r3, 16
    mr r3, r25
    subfic r0, r4, 0x3e7
    clrlwi r0, r0, 16
    cmplw r0, r28
    bge @L_80029DC8
    b @L_80029DCC
    @L_80029DC8:
    mr r0, r28
    @L_80029DCC:
    add r0, r4, r0
    clrlwi r4, r0, 16
    bl fn_80143B70
    b @L_80029EA0
    @L_80029DDC:
    li r29, 0x0
    b @L_80029E90
    @L_80029DE4:
    extsh r0, r29
    clrlwi r28, r28, 16
    cmpwi r0, 0x0
    blt @L_80029E8C
    cmpw r0, r25
    blt @L_80029E00
    b @L_80029E8C
    @L_80029E00:
    slwi r0, r0, 2
    add r24, r30, r0
    mr r3, r24
    bl fn_80143C68
    clrlwi r4, r3, 16
    clrlwi r0, r26, 16
    cmplw r4, r0
    beq @L_80029E2C
    cmplwi r4, 0x0
    beq @L_80029E2C
    b @L_80029E8C
    @L_80029E2C:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    bne @L_80029E4C
    mr r3, r24
    mr r4, r26
    bl fn_80143B80
    li r3, 0x0
    b @L_80029E54
    @L_80029E4C:
    mr r3, r24
    bl fn_80143C50
    @L_80029E54:
    clrlwi r4, r3, 16
    mr r3, r24
    subfic r0, r4, 0x3e7
    clrlwi r0, r0, 16
    cmplw r0, r28
    bge @L_80029E70
    b @L_80029E74
    @L_80029E70:
    mr r0, r28
    @L_80029E74:
    clrlwi r24, r0, 16
    add r0, r4, r24
    clrlwi r4, r0, 16
    bl fn_80143B70
    subf r0, r24, r28
    clrlwi r28, r0, 16
    @L_80029E8C:
    addi r29, r29, 0x1
    @L_80029E90:
    cmpw r29, r25
    bge @L_80029EA0
    cmpwi r28, 0x0
    bgt @L_80029DE4
    @L_80029EA0:
    addi r27, r27, 0x4
    addi r31, r31, 0x1
    @L_80029EA8:
    lhz r25, 0x16(r1)
    cmpw r31, r25
    blt @L_80029D24
    lwz r4, 0x8(r1)
    li r0, 0x0
    li r3, 0x1
    stw r4, 0x758(r30)
    lwz r4, 0xc(r1)
    stw r4, 0x75c(r30)
    stb r0, 0x760(r30)
    lwz r0, 0x10(r1)
    stw r0, 0x764(r30)
    lhz r0, 0x16(r1)
    sth r0, 0x768(r30)
    @L_80029EE0:
    lmw r24, 0xe0(r1)
    lwz r0, 0x104(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
