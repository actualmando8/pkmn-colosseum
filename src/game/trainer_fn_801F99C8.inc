    nofralloc
    stwu r1, -0x790(r1)
    mflr r0
    stw r0, 0x794(r1)
    stmw r25, 0x774(r1)
    mr. r28, r3
    mr r29, r4
    mr r30, r5
    beq @L_801F9CA8
    bne @L_801F99F4
    li r0, 0x0
    b @L_801F9A6C
    @L_801F99F4:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F9A0C
    li r0, 0x0
    b @L_801F9A6C
    @L_801F9A0C:
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_801F9A30
    li r0, 0x0
    b @L_801F9A6C
    @L_801F9A30:
    mr r3, r28
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F9A54
    li r0, 0x0
    b @L_801F9A6C
    @L_801F9A54:
    bl fn_8012A130
    clrlwi. r0, r3, 24
    bne @L_801F9A68
    li r0, 0x0
    b @L_801F9A6C
    @L_801F9A68:
    li r0, 0x1
    @L_801F9A6C:
    clrlwi. r0, r0, 24
    beq @L_801F9CA8
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r26, r3, 16
    li r3, 0x0
    mr r4, r26
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r27, r3, 16
    mr r3, r28
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r27, 0x0
    mr r31, r3
    beq @L_801F9CA8
    mr r4, r26
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F9AEC
    li r0, 0x0
    b @L_801F9B08
    @L_801F9AEC:
    cmplwi r0, 0x2
    beq @L_801F9AFC
    cmplwi r0, 0x3
    bne @L_801F9B04
    @L_801F9AFC:
    li r0, 0x1
    b @L_801F9B08
    @L_801F9B04:
    li r0, 0x2
    @L_801F9B08:
    clrlwi. r0, r0, 24
    beq @L_801F9CA8
    mr r4, r26
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F9B38
    li r0, 0x0
    b @L_801F9B54
    @L_801F9B38:
    cmplwi r0, 0x2
    beq @L_801F9B48
    cmplwi r0, 0x3
    bne @L_801F9B50
    @L_801F9B48:
    li r0, 0x1
    b @L_801F9B54
    @L_801F9B50:
    li r0, 0x2
    @L_801F9B54:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F9B64
    b @L_801F9CA8
    @L_801F9B64:
    addi r27, r1, 0x8
    li r25, 0x0
    li r26, 0x0
    b @L_801F9B94
    @L_801F9B74:
    clrlwi r0, r25, 24
    clrlslwi r4, r25, 24, 2
    mulli r0, r0, 0x138
    addi r3, r1, 0x20
    stwx r26, r27, r4
    add r3, r3, r0
    bl fn_80124A60
    addi r25, r25, 0x1
    @L_801F9B94:
    clrlwi r0, r25, 24
    cmplwi r0, 0x6
    blt @L_801F9B74
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    ble @L_801F9BB0
    li r29, 0x6
    @L_801F9BB0:
    mr r3, r28
    mr r4, r31
    mr r6, r29
    mr r7, r30
    addi r5, r1, 0x8
    bl fn_802331F4
    clrlwi r26, r29, 16
    addi r28, r1, 0x8
    li r25, 0x0
    li r27, 0x0
    b @L_801F9C38
    @L_801F9BDC:
    clrlslwi r0, r27, 24, 2
    lwzx r29, r28, r0
    cmplwi r29, 0x0
    beq @L_801F9C34
    mr r3, r29
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801F9C34
    clrlwi r0, r25, 24
    addi r3, r1, 0x20
    mulli r5, r0, 0x138
    li r0, 0x27
    subi r4, r29, 0x4
    add r5, r3, r5
    mtctr r0
    subi r5, r5, 0x4
    @L_801F9C1C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801F9C1C
    addi r25, r25, 0x1
    @L_801F9C34:
    addi r27, r27, 0x1
    @L_801F9C38:
    clrlwi r0, r27, 24
    cmpw r0, r26
    blt @L_801F9BDC
    addi r29, r1, 0x20
    li r26, 0x0
    li r28, 0x27
    b @L_801F9C9C
    @L_801F9C54:
    mr r3, r31
    clrlwi r5, r26, 24
    li r4, 0x3
    bl fn_8012A5B0
    cmplwi r3, 0x0
    beq @L_801F9C98
    clrlwi r0, r26, 24
    subi r5, r3, 0x4
    mulli r0, r0, 0x138
    add r4, r29, r0
    mtctr r28
    subi r4, r4, 0x4
    @L_801F9C84:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801F9C84
    @L_801F9C98:
    addi r26, r26, 0x1
    @L_801F9C9C:
    clrlwi r0, r26, 24
    cmplwi r0, 0x6
    blt @L_801F9C54
    @L_801F9CA8:
    lmw r25, 0x774(r1)
    lwz r0, 0x794(r1)
    mtlr r0
    addi r1, r1, 0x790
    blr
