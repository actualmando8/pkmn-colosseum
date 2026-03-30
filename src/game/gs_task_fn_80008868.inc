    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    lwz r3, 0x4(r3)
    li r25, 0x28
    bl fn_8005D9E4
    lwz r4, lbl_80478F98(r13)
    li r0, 0x0
    stw r0, 0x8(r1)
    li r0, 0x1e0
    lwz r4, 0x0(r4)
    clrlwi r24, r3, 24
    stw r0, 0x8(r1)
    cmpw r4, r24
    mr r26, r4
    ble @L_800088B0
    mr r26, r24
    @L_800088B0:
    mulli r27, r26, 0xd
    lwz r4, 0x8(r1)
    li r3, 0x1e
    addi r5, r4, 0x19
    mr r6, r27
    li r4, 0x28
    bl fn_8001EA98
    lha r0, lbl_80478838(r13)
    cmpwi r0, 0x0
    ble @L_80008904
    lwz r3, 0x8(r1)
    li r4, 0x21
    li r6, 0x21
    li r8, 0x17
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r7, r0, 1
    addi r3, r7, 0x14
    addi r5, r7, 0x28
    addi r7, r7, 0x1e
    bl fn_8001E4B4
    @L_80008904:
    lwz r3, lbl_80478F98(r13)
    lha r4, lbl_80478838(r13)
    lwz r0, 0x0(r3)
    subf r0, r4, r0
    cmpw r0, r24
    ble @L_80008948
    lwz r3, 0x8(r1)
    addi r4, r27, 0x2f
    mr r6, r4
    addi r8, r27, 0x39
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r7, r0, 1
    addi r3, r7, 0x14
    addi r5, r7, 0x28
    addi r7, r7, 0x1e
    bl fn_8001E4B4
    @L_80008948:
    li r27, 0x0
    la r28, lbl_80478838(r13)
    lis r4, lbl_803A19C8@ha
    lis r3, lbl_80266688@ha
    lwz r31, 0x8(r1)
    addi r29, r4, lbl_803A19C8@l
    addi r30, r3, lbl_80266688@l
    b @L_80008C24
    @L_80008968:
    lha r0, 0x2(r28)
    cmpw r0, r27
    bne @L_80008980
    lis r3, 0xff00
    addi r24, r3, 0xff
    b @L_80008984
    @L_80008980:
    li r24, -0x1
    @L_80008984:
    lha r0, lbl_80478838(r13)
    add r5, r27, r0
    cmpwi r5, 0x0
    bge @L_8000899C
    li r0, 0x0
    b @L_80008AA8
    @L_8000899C:
    lwz r3, lbl_80478F98(r13)
    lwz r0, 0x0(r3)
    cmplw r5, r0
    blt @L_800089B4
    li r0, 0x0
    b @L_80008AA8
    @L_800089B4:
    cmpwi r5, 0x0
    lha r7, 0x4(r3)
    beq @L_80008AA4
    subi r0, r5, 0x1
    li r3, 0x0
    cmpwi r0, 0x0
    ble @L_80008A90
    cmpwi r0, 0x8
    subi r6, r5, 0x9
    ble @L_80008A60
    lwz r4, lbl_80478F9C(r13)
    addi r0, r6, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r6, 0x0
    ble @L_80008A60
    @L_800089F4:
    extsh r0, r7
    addi r3, r3, 0x8
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r4, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r7, r4, r0
    bdnz @L_800089F4
    @L_80008A60:
    lwz r6, lbl_80478F9C(r13)
    subi r4, r5, 0x1
    subf r0, r3, r4
    mtctr r0
    cmpw r3, r4
    bge @L_80008A90
    @L_80008A78:
    extsh r0, r7
    addi r3, r3, 0x1
    slwi r4, r0, 3
    addi r0, r4, 0x6
    lhax r7, r6, r0
    bdnz @L_80008A78
    @L_80008A90:
    extsh r0, r7
    lwz r4, lbl_80478F9C(r13)
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r4, r0
    @L_80008AA4:
    clrlwi r0, r7, 16
    @L_80008AA8:
    mr r3, r29
    mr r4, r30
    clrlwi r6, r0, 16
    crxor 6, 6, 6
    bl fn_800C8520
    mr r4, r25
    mr r5, r24
    mr r6, r29
    li r3, 0x1e
    crxor 6, 6, 6
    bl fn_800FAEF8
    lha r0, lbl_80478838(r13)
    add r4, r27, r0
    cmpwi r4, 0x0
    bge @L_80008AEC
    li r0, 0x0
    b @L_80008BF8
    @L_80008AEC:
    lwz r3, lbl_80478F98(r13)
    lwz r0, 0x0(r3)
    cmplw r4, r0
    blt @L_80008B04
    li r0, 0x0
    b @L_80008BF8
    @L_80008B04:
    cmpwi r4, 0x0
    lha r7, 0x4(r3)
    beq @L_80008BF4
    subi r0, r4, 0x1
    li r3, 0x0
    cmpwi r0, 0x0
    ble @L_80008BE0
    cmpwi r0, 0x8
    subi r6, r4, 0x9
    ble @L_80008BB0
    lwz r5, lbl_80478F9C(r13)
    addi r0, r6, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r6, 0x0
    ble @L_80008BB0
    @L_80008B44:
    extsh r0, r7
    addi r3, r3, 0x8
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r0, r5, r0
    slwi r6, r0, 3
    addi r0, r6, 0x6
    lhax r7, r5, r0
    bdnz @L_80008B44
    @L_80008BB0:
    lwz r5, lbl_80478F9C(r13)
    subi r4, r4, 0x1
    subf r0, r3, r4
    mtctr r0
    cmpw r3, r4
    bge @L_80008BE0
    @L_80008BC8:
    extsh r0, r7
    addi r3, r3, 0x1
    slwi r4, r0, 3
    addi r0, r4, 0x6
    lhax r7, r5, r0
    bdnz @L_80008BC8
    @L_80008BE0:
    extsh r0, r7
    lwz r4, lbl_80478F9C(r13)
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r4, r0
    @L_80008BF4:
    clrlwi r0, r7, 16
    @L_80008BF8:
    clrlwi r3, r0, 16
    bl fn_801906A0
    mr r7, r3
    mr r4, r25
    mr r5, r24
    addi r3, r31, 0x1e
    la r6, lbl_8047B6C8(r2)
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r25, r25, 0xd
    addi r27, r27, 0x1
    @L_80008C24:
    cmpw r27, r26
    blt @L_80008968
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
