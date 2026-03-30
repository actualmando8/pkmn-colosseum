    nofralloc
    mflr r0
    lis r7, 0xcc00
    stw r0, 0x4(r1)
    stwu r1, -0x40(r1)
    stmw r22, 0x18(r1)
    addi r25, r4, 0x0
    addi r4, r7, 0x6000
    lis r7, CommandList_803FC290@ha
    addi r31, r7, CommandList_803FC290@l
    addi r24, r3, 0x0
    addi r26, r5, 0x0
    addi r30, r31, 0xcc
    mr r27, r6
    stw r25, 0x18(r4)
    stw r24, 0xc4(r31)
    stw r25, 0xc8(r31)
    stw r26, 0xcc(r31)
    lwz r0, WorkAroundType_8047A7A4(r13)
    cmplwi r0, 0x0
    bne @L_800A44CC
    li r0, -0x1
    stw r0, 0x0(r31)
    li r0, 0x0
    addi r3, r24, 0x0
    stw r0, NextCommandNumber_8047A7C4(r13)
    addi r4, r25, 0x0
    addi r5, r26, 0x0
    addi r6, r27, 0x0
    bl fn_800A42C4
    b @L_800A46D4
    @L_800A44CC:
    lwz r0, WorkAroundType_8047A7A4(r13)
    cmplwi r0, 0x1
    bne @L_800A46D4
    lwz r0, lbl_804789B8(r13)
    cmpwi r0, 0x0
    beq @L_800A44FC
    addi r3, r24, 0x0
    addi r4, r25, 0x0
    addi r5, r26, 0x0
    addi r6, r27, 0x0
    bl fn_800A43D4
    b @L_800A46D4
    @L_800A44FC:
    addi r29, r31, 0xbc
    lwz r0, 0x0(r30)
    addi r28, r31, 0xc0
    lwz r3, 0xbc(r31)
    lwz r4, 0xc0(r31)
    srwi r23, r0, 15
    subi r0, r3, 0x1
    add r0, r4, r0
    srwi r22, r0, 15
    bl fn_800A7BCC
    lbz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_800A4538
    li r0, 0x1
    b @L_800A453C
    @L_800A4538:
    li r0, 0x0
    @L_800A453C:
    cmpwi r0, 0x0
    beq @L_800A454C
    li r3, 0x5
    b @L_800A4550
    @L_800A454C:
    li r3, 0xf
    @L_800A4550:
    subi r0, r22, 0x2
    cmplw r23, r0
    bgt @L_800A456C
    addi r0, r3, 0x3
    add r0, r22, r0
    cmplw r23, r0
    bge @L_800A4574
    @L_800A456C:
    li r0, 0x1
    b @L_800A4578
    @L_800A4574:
    li r0, 0x0
    @L_800A4578:
    cmpwi r0, 0x0
    bne @L_800A45A8
    li r0, -0x1
    stw r0, 0x0(r31)
    li r0, 0x0
    addi r3, r24, 0x0
    stw r0, NextCommandNumber_8047A7C4(r13)
    addi r4, r25, 0x0
    addi r5, r26, 0x0
    addi r6, r27, 0x0
    bl fn_800A42C4
    b @L_800A46D4
    @L_800A45A8:
    lwz r3, 0x0(r29)
    lwz r4, 0x0(r28)
    subi r3, r3, 0x1
    lwz r0, 0x0(r30)
    add r3, r4, r3
    srwi r3, r3, 15
    srwi r4, r0, 15
    cmplw r3, r4
    beq @L_800A45D8
    addi r0, r3, 0x1
    cmplw r0, r4
    bne @L_800A46C0
    @L_800A45D8:
    bl __OSGetSystemTime
    lis r5, 0x8000
    lwz r8, lbl_8047A7B0(r13)
    lwz r0, 0xf8(r5)
    lis r5, 0x1062
    lwz r9, lbl_8047A7B4(r13)
    li r6, 0x0
    srwi r7, r0, 2
    addi r0, r5, 0x4dd3
    mulhwu r0, r0, r7
    srwi r0, r0, 6
    subfc r9, r9, r4
    subfe r8, r8, r3
    mulli r5, r0, 0x5
    xoris r4, r6, 0x8000
    xoris r3, r8, 0x8000
    subfc r0, r9, r5
    subfe r3, r3, r4
    subfe r3, r4, r4
    neg. r3, r3
    beq @L_800A4650
    li r0, -0x1
    stw r0, 0x0(r31)
    addi r3, r24, 0x0
    addi r4, r25, 0x0
    stw r6, NextCommandNumber_8047A7C4(r13)
    addi r5, r26, 0x0
    addi r6, r27, 0x0
    bl fn_800A42C4
    b @L_800A46D4
    @L_800A4650:
    li r0, 0x1
    stw r0, 0x0(r31)
    lis r3, 0x431c
    subi r0, r3, 0x217d
    stw r24, 0x4(r31)
    mulhwu r0, r0, r7
    stw r25, 0x8(r31)
    stw r26, 0xc(r31)
    srwi r0, r0, 15
    mulli r3, r0, 0x1f4
    stw r27, 0x10(r31)
    li r0, -0x1
    stw r0, 0x14(r31)
    subfc r5, r9, r5
    subfe r4, r8, r6
    srwi r0, r3, 3
    stw r6, NextCommandNumber_8047A7C4(r13)
    addc r23, r5, r0
    adde r22, r4, r6
    addi r3, r31, 0x40
    bl OSCreateAlarm
    lis r3, fn_800A41D0@ha
    addi r7, r3, fn_800A41D0@l
    addi r6, r23, 0x0
    addi r5, r22, 0x0
    addi r3, r31, 0x40
    bl OSSetAlarm
    b @L_800A46D4
    @L_800A46C0:
    addi r3, r24, 0x0
    addi r4, r25, 0x0
    addi r5, r26, 0x0
    addi r6, r27, 0x0
    bl fn_800A43D4
    @L_800A46D4:
    lmw r22, 0x18(r1)
    li r3, 0x1
    lwz r0, 0x44(r1)
    addi r1, r1, 0x40
    mtlr r0
    blr
