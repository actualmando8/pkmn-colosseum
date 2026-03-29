    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r29, r3
    mr r30, r4
    mr r26, r5
    mr r31, r6
    cmplwi r31, 0x0
    beq @L_80029CAC
    lhz r28, 0x768(r31)
    cmpwi r28, -0x1
    ble @L_80029BC4
    clrlwi r24, r26, 16
    li r27, 0x0
    b @L_80029BB4
    @L_80029B08:
    extsh r0, r27
    clrlwi r24, r24, 16
    cmpwi r0, 0x0
    blt @L_80029BB0
    cmpw r0, r28
    blt @L_80029B24
    b @L_80029BB0
    @L_80029B24:
    slwi r0, r0, 2
    add r25, r31, r0
    mr r3, r25
    bl fn_80143C68
    clrlwi r4, r3, 16
    clrlwi r0, r30, 16
    cmplw r4, r0
    beq @L_80029B50
    cmplwi r4, 0x0
    beq @L_80029B50
    b @L_80029BB0
    @L_80029B50:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    bne @L_80029B70
    mr r3, r25
    mr r4, r30
    bl fn_80143B80
    li r3, 0x0
    b @L_80029B78
    @L_80029B70:
    mr r3, r25
    bl fn_80143C50
    @L_80029B78:
    clrlwi r4, r3, 16
    mr r3, r25
    subfic r0, r4, 0x3e7
    clrlwi r0, r0, 16
    cmplw r0, r24
    bge @L_80029B94
    b @L_80029B98
    @L_80029B94:
    mr r0, r24
    @L_80029B98:
    clrlwi r25, r0, 16
    add r0, r4, r25
    clrlwi r4, r0, 16
    bl fn_80143B70
    subf r0, r25, r24
    clrlwi r24, r0, 16
    @L_80029BB0:
    addi r27, r27, 0x1
    @L_80029BB4:
    cmpw r27, r28
    bge @L_80029BC4
    cmpwi r24, 0x0
    bgt @L_80029B08
    @L_80029BC4:
    cmpwi r28, -0x1
    ble @L_80029C98
    clrlwi r25, r26, 16
    li r27, 0x0
    b @L_80029C88
    @L_80029BD8:
    extsh r0, r27
    clrlwi r25, r25, 16
    cmpwi r0, 0x0
    addi r24, r31, 0x3ac
    blt @L_80029C84
    cmpw r0, r28
    blt @L_80029BF8
    b @L_80029C84
    @L_80029BF8:
    slwi r0, r0, 2
    add r24, r24, r0
    mr r3, r24
    bl fn_80143C68
    clrlwi r4, r3, 16
    clrlwi r0, r30, 16
    cmplw r4, r0
    beq @L_80029C24
    cmplwi r4, 0x0
    beq @L_80029C24
    b @L_80029C84
    @L_80029C24:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    bne @L_80029C44
    mr r3, r24
    mr r4, r30
    bl fn_80143B80
    li r3, 0x0
    b @L_80029C4C
    @L_80029C44:
    mr r3, r24
    bl fn_80143C50
    @L_80029C4C:
    clrlwi r4, r3, 16
    mr r3, r24
    subfic r0, r4, 0x3e7
    clrlwi r0, r0, 16
    cmplw r0, r25
    bge @L_80029C68
    b @L_80029C6C
    @L_80029C68:
    mr r0, r25
    @L_80029C6C:
    clrlwi r24, r0, 16
    add r0, r4, r24
    clrlwi r4, r0, 16
    bl fn_80143B70
    subf r0, r24, r25
    clrlwi r25, r0, 16
    @L_80029C84:
    addi r27, r27, 0x1
    @L_80029C88:
    cmpw r27, r28
    bge @L_80029C98
    cmpwi r25, 0x0
    bgt @L_80029BD8
    @L_80029C98:
    lwz r3, 0x758(r31)
    li r0, 0x1
    subf r3, r29, r3
    stw r3, 0x758(r31)
    stb r0, 0x760(r31)
    @L_80029CAC:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
