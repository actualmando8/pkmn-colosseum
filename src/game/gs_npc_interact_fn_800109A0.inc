    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80010A1C
    li r4, 0x48
    bl fn_80103FFC
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80010A00
    mr r3, r31
    li r4, 0x0
    bl fn_801040D0
    mr r4, r3
    mr r3, r28
    li r5, 0x48
    bl memcpy
    @L_80010A00:
    mr r3, r31
    bl fn_801040A0
    li r4, 0x0
    li r0, -0x1
    stb r4, 0x0(r3)
    stb r4, 0x1(r3)
    stb r0, 0x2(r3)
    @L_80010A1C:
    mr r3, r31
    bl fn_80103FE4
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x5
    bne @L_80010A5C
    li r28, 0x0
    la r30, lbl_80478850(r13)
    @L_80010A3C:
    lhz r3, 0x0(r30)
    li r4, 0x1
    bl fn_8005D8F8
    addi r30, r30, 0x2
    addi r28, r28, 0x1
    cmpwi r28, 0x4
    blt @L_80010A3C
    b @L_80010AA4
    @L_80010A5C:
    mr r30, r3
    li r28, 0x0
    la r29, lbl_80478850(r13)
    @L_80010A68:
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_80010A84
    lhz r3, 0x0(r29)
    li r4, 0x1
    bl fn_8005D8F8
    b @L_80010A90
    @L_80010A84:
    lhz r3, 0x0(r29)
    li r4, 0x0
    bl fn_8005D8F8
    @L_80010A90:
    addi r30, r30, 0xc
    addi r29, r29, 0x2
    addi r28, r28, 0x1
    cmpwi r28, 0x4
    blt @L_80010A68
    @L_80010AA4:
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xc6
    beq @L_80010AEC
    bge @L_80010AC8
    cmpwi r3, 0xc4
    beq @L_80010AD4
    bge @L_80010AE0
    b @L_80010B04
    @L_80010AC8:
    cmpwi r3, 0xc8
    bge @L_80010B04
    b @L_80010AF8
    @L_80010AD4:
    li r0, 0x0
    stw r0, 0x80(r31)
    b @L_80010B0C
    @L_80010AE0:
    li r0, 0x1
    stw r0, 0x80(r31)
    b @L_80010B0C
    @L_80010AEC:
    li r0, 0x2
    stw r0, 0x80(r31)
    b @L_80010B0C
    @L_80010AF8:
    li r0, 0x3
    stw r0, 0x80(r31)
    b @L_80010B0C
    @L_80010B04:
    li r0, -0x1
    stw r0, 0x80(r31)
    @L_80010B0C:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
