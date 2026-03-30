    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r28, r3
    bl fn_80105624
    lhz r27, 0x6(r3)
    bl fn_80105624
    lwz r4, lbl_80478F98(r13)
    lhz r31, 0x0(r3)
    lwz r0, 0x0(r4)
    lwz r3, 0x4(r28)
    extsh r30, r0
    bl fn_8005D9E4
    clrlwi r29, r3, 24
    extsh r0, r29
    cmpw r30, r0
    bge @L_80008C8C
    mr r29, r30
    @L_80008C8C:
    clrlwi r4, r27, 16
    lwz r3, lbl_80478838(r13)
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    stw r3, 0x8(r1)
    beq @L_80008CB4
    lha r3, 0xa(r1)
    subi r0, r3, 0x1
    sth r0, 0xa(r1)
    b @L_80008CCC
    @L_80008CB4:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80008CCC
    lha r3, 0xa(r1)
    addi r0, r3, 0x1
    sth r0, 0xa(r1)
    @L_80008CCC:
    rlwinm r0, r4, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80008CFC
    lwz r3, 0x4(r28)
    bl fn_8005D9E4
    clrlwi r3, r3, 24
    lha r0, 0xa(r1)
    subi r3, r3, 0x1
    subf r0, r3, r0
    extsh r0, r0
    sth r0, 0xa(r1)
    b @L_80008D28
    @L_80008CFC:
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80008D28
    lwz r3, 0x4(r28)
    bl fn_8005D9E4
    lha r0, 0xa(r1)
    clrlwi r3, r3, 24
    add r3, r3, r0
    subi r0, r3, 0x1
    extsh r0, r0
    sth r0, 0xa(r1)
    @L_80008D28:
    lha r4, 0xa(r1)
    cmpwi r4, 0x0
    bge @L_80008D74
    lha r3, 0x8(r1)
    li r0, 0x0
    sth r0, 0xa(r1)
    add r3, r3, r4
    extsh r0, r3
    sth r3, 0x8(r1)
    cmpwi r0, 0x0
    bge @L_80008DB8
    extsh r4, r29
    subi r3, r4, 0x1
    subf r0, r4, r30
    extsh r3, r3
    extsh r0, r0
    sth r3, 0xa(r1)
    sth r0, 0x8(r1)
    b @L_80008DB8
    @L_80008D74:
    extsh r3, r29
    cmpw r4, r3
    blt @L_80008DB8
    subi r5, r3, 0x1
    lha r3, 0x8(r1)
    subf r0, r5, r4
    add r3, r3, r0
    extsh r0, r5
    extsh r3, r3
    sth r0, 0xa(r1)
    add r0, r3, r0
    cmpw r0, r30
    sth r3, 0x8(r1)
    blt @L_80008DB8
    li r0, 0x0
    sth r0, 0x8(r1)
    sth r0, 0xa(r1)
    @L_80008DB8:
    clrlwi r3, r31, 16
    rlwinm r0, r3, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_80008EF8
    lha r3, 0x8(r1)
    lha r0, 0xa(r1)
    add r5, r3, r0
    cmpwi r5, 0x0
    bge @L_80008DE4
    li r0, 0x0
    b @L_80008EEC
    @L_80008DE4:
    lwz r3, lbl_80478F98(r13)
    lwz r0, 0x0(r3)
    cmplw r5, r0
    blt @L_80008DFC
    li r0, 0x0
    b @L_80008EEC
    @L_80008DFC:
    cmpwi r5, 0x0
    lha r7, 0x4(r3)
    beq @L_80008EE8
    subi r0, r5, 0x1
    li r4, 0x0
    cmpwi r0, 0x0
    ble @L_80008ED4
    cmpwi r0, 0x8
    subi r3, r5, 0x9
    ble @L_80008EA8
    lwz r6, lbl_80478F9C(r13)
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_80008EA8
    @L_80008E3C:
    extsh r0, r7
    addi r4, r4, 0x8
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_80008E3C
    @L_80008EA8:
    lwz r6, lbl_80478F9C(r13)
    subi r3, r5, 0x1
    subf r0, r4, r3
    mtctr r0
    cmpw r4, r3
    bge @L_80008ED4
    @L_80008EC0:
    extsh r0, r7
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_80008EC0
    @L_80008ED4:
    extsh r0, r7
    lwz r3, lbl_80478F9C(r13)
    slwi r0, r0, 3
    add r3, r3, r0
    lha r7, 0x6(r3)
    @L_80008EE8:
    clrlwi r0, r7, 16
    @L_80008EEC:
    clrlwi r3, r0, 16
    bl fn_80190528
    b @L_80009030
    @L_80008EF8:
    rlwinm r0, r3, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_80009030
    lha r3, 0x8(r1)
    lha r0, 0xa(r1)
    add r5, r3, r0
    cmpwi r5, 0x0
    bge @L_80008F20
    li r0, 0x0
    b @L_80009028
    @L_80008F20:
    lwz r3, lbl_80478F98(r13)
    lwz r0, 0x0(r3)
    cmplw r5, r0
    blt @L_80008F38
    li r0, 0x0
    b @L_80009028
    @L_80008F38:
    cmpwi r5, 0x0
    lha r7, 0x4(r3)
    beq @L_80009024
    subi r0, r5, 0x1
    li r4, 0x0
    cmpwi r0, 0x0
    ble @L_80009010
    cmpwi r0, 0x8
    subi r3, r5, 0x9
    ble @L_80008FE4
    lwz r6, lbl_80478F9C(r13)
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_80008FE4
    @L_80008F78:
    extsh r0, r7
    addi r4, r4, 0x8
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_80008F78
    @L_80008FE4:
    lwz r6, lbl_80478F9C(r13)
    subi r3, r5, 0x1
    subf r0, r4, r3
    mtctr r0
    cmpw r4, r3
    bge @L_80009010
    @L_80008FFC:
    extsh r0, r7
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_80008FFC
    @L_80009010:
    extsh r0, r7
    lwz r3, lbl_80478F9C(r13)
    slwi r0, r0, 3
    add r3, r3, r0
    lha r7, 0x6(r3)
    @L_80009024:
    clrlwi r0, r7, 16
    @L_80009028:
    clrlwi r3, r0, 16
    bl fn_801903B0
    @L_80009030:
    lha r3, 0x8(r1)
    lha r0, 0xa(r1)
    lwz r4, 0x8(r1)
    add r5, r3, r0
    cmpwi r5, 0x0
    stw r4, lbl_80478838(r13)
    bge @L_80009054
    li r0, 0x0
    b @L_8000915C
    @L_80009054:
    lwz r3, lbl_80478F98(r13)
    lwz r0, 0x0(r3)
    cmplw r5, r0
    blt @L_8000906C
    li r0, 0x0
    b @L_8000915C
    @L_8000906C:
    cmpwi r5, 0x0
    lha r7, 0x4(r3)
    beq @L_80009158
    subi r0, r5, 0x1
    li r4, 0x0
    cmpwi r0, 0x0
    ble @L_80009144
    cmpwi r0, 0x8
    subi r3, r5, 0x9
    ble @L_80009118
    lwz r6, lbl_80478F9C(r13)
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_80009118
    @L_800090AC:
    extsh r0, r7
    addi r4, r4, 0x8
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r0, r6, r0
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_800090AC
    @L_80009118:
    lwz r6, lbl_80478F9C(r13)
    subi r3, r5, 0x1
    subf r0, r4, r3
    mtctr r0
    cmpw r4, r3
    bge @L_80009144
    @L_80009130:
    extsh r0, r7
    slwi r3, r0, 3
    addi r0, r3, 0x6
    lhax r7, r6, r0
    bdnz @L_80009130
    @L_80009144:
    extsh r0, r7
    lwz r3, lbl_80478F9C(r13)
    slwi r0, r0, 3
    add r3, r3, r0
    lha r7, 0x6(r3)
    @L_80009158:
    clrlwi r0, r7, 16
    @L_8000915C:
    clrlwi r0, r0, 16
    stw r0, 0x80(r28)
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
