    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r3
    bl fn_801040A0
    mr r30, r3
    mr r3, r26
    li r29, 0x0
    li r4, 0x0
    bl fn_801040D0
    mr r0, r3
    mr r3, r26
    mr r27, r0
    li r4, 0x1
    bl fn_801040D0
    mr r31, r3
    cmpwi r31, 0x2
    bne @L_80013008
    li r25, 0x8
    li r28, 0x10
    b @L_80013010
    @L_80013008:
    li r25, 0xa
    li r28, 0xa
    @L_80013010:
    bl fn_80105624
    lhz r4, 0x6(r3)
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80013034
    lwz r3, 0x0(r30)
    subi r0, r3, 0x1
    stw r0, 0x0(r30)
    b @L_8001304C
    @L_80013034:
    rlwinm r0, r4, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8001304C
    lwz r3, 0x0(r30)
    addi r0, r3, 0x1
    stw r0, 0x0(r30)
    @L_8001304C:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    bge @L_80013064
    li r0, 0x0
    li r27, 0x0
    stw r0, 0x0(r30)
    @L_80013064:
    lwz r0, 0x0(r30)
    cmpw r0, r25
    blt @L_80013078
    subi r0, r25, 0x1
    stw r0, 0x0(r30)
    @L_80013078:
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    beq @L_800130FC
    lwz r4, 0x0(r30)
    li r29, 0x1
    li r5, 0x0
    cmpwi r4, 0x0
    ble @L_8001317C
    cmpwi r4, 0x8
    subi r3, r4, 0x8
    ble @L_800130E0
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_800130E0
    @L_800130B8:
    mullw r29, r29, r28
    addi r5, r5, 0x8
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    bdnz @L_800130B8
    @L_800130E0:
    subf r0, r5, r4
    mtctr r0
    cmpw r5, r4
    bge @L_8001317C
    @L_800130F0:
    mullw r29, r29, r28
    bdnz @L_800130F0
    b @L_8001317C
    @L_800130FC:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8001317C
    lwz r4, 0x0(r30)
    li r29, -0x1
    li r5, 0x0
    cmpwi r4, 0x0
    ble @L_8001317C
    cmpwi r4, 0x8
    subi r3, r4, 0x8
    ble @L_80013164
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_80013164
    @L_8001313C:
    mullw r29, r29, r28
    addi r5, r5, 0x8
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    mullw r29, r29, r28
    bdnz @L_8001313C
    @L_80013164:
    subf r0, r5, r4
    mtctr r0
    cmpw r5, r4
    bge @L_8001317C
    @L_80013174:
    mullw r29, r29, r28
    bdnz @L_80013174
    @L_8001317C:
    cmpwi r31, 0x0
    bne @L_800131FC
    srawi r6, r27, 31
    li r0, -0x1
    srawi r4, r29, 31
    lis r5, 0x8000
    addc r7, r27, r29
    xoris r3, r0, 0x8000
    adde r6, r6, r4
    xoris r4, r6, 0x8000
    subfc r0, r5, r7
    subfe r3, r3, r4
    subfe r3, r4, r4
    neg r3, r3
    cmpwi r3, 0x0
    beq @L_800131C4
    lis r7, 0x8000
    li r6, -0x1
    @L_800131C4:
    lis r3, 0x8000
    li r0, 0x0
    subi r5, r3, 0x1
    xoris r3, r6, 0x8000
    xoris r4, r0, 0x8000
    subfc r0, r7, r5
    subfe r3, r3, r4
    subfe r3, r4, r4
    neg r3, r3
    cmpwi r3, 0x0
    beq @L_8001326C
    mr r7, r5
    li r6, 0x0
    b @L_8001326C
    @L_800131FC:
    srawi r0, r29, 31
    li r7, 0x0
    addc r6, r27, r29
    adde r5, r7, r0
    xoris r3, r7, 0x8000
    xoris r4, r5, 0x8000
    subfc r0, r7, r6
    subfe r3, r3, r4
    mr r7, r6
    subfe r3, r4, r4
    mr r6, r5
    neg r3, r3
    cmpwi r3, 0x0
    beq @L_8001323C
    li r7, 0x0
    li r6, 0x0
    @L_8001323C:
    li r5, -0x1
    li r0, 0x0
    xoris r4, r0, 0x8000
    xoris r3, r6, 0x8000
    subfc r0, r7, r5
    subfe r3, r3, r4
    subfe r3, r4, r4
    neg r3, r3
    cmpwi r3, 0x0
    beq @L_8001326C
    li r7, -0x1
    li r6, 0x0
    @L_8001326C:
    li r0, -0x1
    mr r3, r26
    and r5, r7, r0
    li r4, 0x0
    stw r5, 0x80(r26)
    bl fn_801040B8
    li r3, 0x0
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
