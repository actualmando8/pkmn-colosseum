    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_801040A0
    lbz r0, 0x1(r28)
    mr r31, r3
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80012A00
    mr r3, r28
    li r4, 0x30
    bl fn_80103FFC
    cmplwi r3, 0x0
    beq @L_80012A00
    lwz r4, 0x60(r28)
    li r5, 0x30
    bl memcpy
    @L_80012A00:
    mr r3, r28
    bl fn_80103FE4
    lbz r0, 0x1(r28)
    lwz r29, 0x4(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80012AD0
    mr r3, r29
    li r30, 0x0
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80012A74
    cmpwi r29, 0x49
    beq @L_80012A5C
    bge @L_80012A50
    cmpwi r29, 0x47
    bge @L_80012A64
    cmpwi r29, 0x45
    bge @L_80012A5C
    b @L_80012A68
    @L_80012A50:
    cmpwi r29, 0x4b
    bge @L_80012A68
    b @L_80012A64
    @L_80012A5C:
    li r30, 0x538
    b @L_80012A68
    @L_80012A64:
    li r30, 0x540
    @L_80012A68:
    mr r4, r30
    li r5, 0x0
    bl fn_80103F74
    @L_80012A74:
    lwz r29, 0x4(r28)
    li r30, 0x0
    mr r3, r29
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80012AD0
    cmpwi r29, 0x49
    beq @L_80012AB8
    bge @L_80012AAC
    cmpwi r29, 0x47
    bge @L_80012AC0
    cmpwi r29, 0x45
    bge @L_80012AB8
    b @L_80012AC4
    @L_80012AAC:
    cmpwi r29, 0x4b
    bge @L_80012AC4
    b @L_80012AC0
    @L_80012AB8:
    li r30, 0x539
    b @L_80012AC4
    @L_80012AC0:
    li r30, 0x541
    @L_80012AC4:
    mr r4, r30
    li r5, 0x0
    bl fn_80103F74
    @L_80012AD0:
    lha r0, 0x2(r31)
    cmpwi r0, 0x0
    beq @L_80012B00
    lha r3, 0x4(r31)
    addi r0, r3, 0x1
    sth r0, 0x4(r31)
    lha r3, 0x4(r31)
    lha r0, 0x2(r31)
    cmpw r3, r0
    ble @L_80012B00
    li r0, 0x0
    sth r0, 0x2(r31)
    @L_80012B00:
    lha r0, 0xc(r31)
    cmpwi r0, 0x0
    beq @L_80012B38
    lha r3, 0xe(r31)
    addi r0, r3, 0x1
    sth r0, 0xe(r31)
    lha r3, 0xe(r31)
    lha r0, 0xc(r31)
    cmpw r3, r0
    ble @L_80012B38
    li r0, 0x0
    li r3, 0x4d0
    sth r0, 0xc(r31)
    bl fn_801669BC
    @L_80012B38:
    lhz r4, 0x6(r31)
    lis r3, 0x1b4f
    subi r0, r3, 0x7e4b
    li r3, 0x0
    addi r4, r4, 0x1
    sth r4, 0x6(r31)
    lhz r5, 0x6(r31)
    mulhw r0, r0, r5
    srawi r0, r0, 7
    srwi r4, r0, 31
    add r0, r0, r4
    mulli r0, r0, 0x4b0
    subf r0, r0, r5
    clrlwi r0, r0, 16
    sth r0, 0x6(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
