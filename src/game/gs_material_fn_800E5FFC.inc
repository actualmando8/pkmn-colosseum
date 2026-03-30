    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lhz r3, 0x152(r3)
    cmplwi r3, 0x0
    beq @L_800E60D0
    lwz r31, 0x14c(r28)
    subi r0, r3, 0x1
    lhz r30, 0x150(r28)
    sth r0, 0x152(r28)
    lhz r0, 0x152(r28)
    cmplwi r0, 0x0
    bne @L_800E60A4
    lhz r30, 0x150(r28)
    li r29, 0x0
    lwz r31, 0x14c(r28)
    b @L_800E606C
    @L_800E6054:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800E6064
    bl fn_800DF608
    @L_800E6064:
    addi r29, r29, 0x1
    addi r31, r31, 0x4
    @L_800E606C:
    cmpw r29, r30
    blt @L_800E6054
    lhz r29, 0x154(r28)
    cmplwi r29, 0x0
    beq @L_800E6090
    mr r3, r29
    bl fn_800E24B0
    mr r3, r29
    bl fn_800E209C
    @L_800E6090:
    li r0, 0x0
    sth r0, 0x154(r28)
    sth r0, 0x150(r28)
    stw r0, 0x14c(r28)
    b @L_800E60D0
    @L_800E60A4:
    li r29, 0x0
    b @L_800E60C8
    @L_800E60AC:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800E60C0
    li r4, 0x4
    bl fn_800DF248
    @L_800E60C0:
    addi r29, r29, 0x1
    addi r31, r31, 0x4
    @L_800E60C8:
    cmpw r29, r30
    blt @L_800E60AC
    @L_800E60D0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
