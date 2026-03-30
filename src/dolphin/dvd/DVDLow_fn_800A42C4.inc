    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    li r0, 0x0
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    addi r30, r5, 0x0
    stw r29, 0x1c(r1)
    addi r29, r4, 0x0
    stw r28, 0x18(r1)
    addi r28, r3, 0x0
    stw r0, StopAtNextInt_8047A780(r13)
    li r0, 0x1
    stw r6, Callback_8047A788(r13)
    lis r6, CommandList_803FC290@ha
    addi r31, r6, CommandList_803FC290@l
    stw r0, lbl_8047A7C0(r13)
    bl __OSGetSystemTime
    stw r4, lbl_8047A7BC(r13)
    lis r4, 0xcc00
    lis r0, 0xa0
    stw r3, lbl_8047A7B8(r13)
    addi r4, r4, 0x6000
    lis r3, 0xa800
    stw r3, 0x8(r4)
    srwi r3, r30, 2
    cmplw r29, r0
    stw r3, 0xc(r4)
    li r0, 0x3
    stw r29, 0x10(r4)
    stw r28, 0x14(r4)
    stw r29, 0x18(r4)
    stw r29, lbl_8047A784(r13)
    stw r0, 0x1c(r4)
    ble @L_800A4384
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    addi r3, r31, 0x68
    srwi r0, r0, 2
    mulli r30, r0, 0x14
    bl OSCreateAlarm
    lis r3, AlarmHandlerForTimeout@ha
    addi r7, r3, AlarmHandlerForTimeout@l
    addi r6, r30, 0x0
    addi r3, r31, 0x68
    li r5, 0x0
    bl OSSetAlarm
    b @L_800A43B4
    @L_800A4384:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    addi r3, r31, 0x68
    srwi r0, r0, 2
    mulli r30, r0, 0xa
    bl OSCreateAlarm
    lis r3, AlarmHandlerForTimeout@ha
    addi r7, r3, AlarmHandlerForTimeout@l
    addi r6, r30, 0x0
    addi r3, r31, 0x68
    li r5, 0x0
    bl OSSetAlarm
    @L_800A43B4:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    lwz r28, 0x18(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
