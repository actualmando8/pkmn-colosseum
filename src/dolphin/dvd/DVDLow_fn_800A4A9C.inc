    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    li r0, 0x0
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    stw r4, Callback_8047A788(r13)
    lis r4, 0xcc00
    addi r4, r4, 0x6000
    stw r0, StopAtNextInt_8047A780(r13)
    oris r0, r3, 0xe200
    lis r3, AlarmForTimeout_803FC2F8@ha
    stw r0, 0x8(r4)
    li r0, 0x1
    addi r31, r3, AlarmForTimeout_803FC2F8@l
    stw r0, 0x1c(r4)
    lis r4, 0x8000
    addi r3, r31, 0x0
    lwz r0, 0xf8(r4)
    srwi r0, r0, 2
    mulli r30, r0, 0xa
    bl OSCreateAlarm
    lis r3, AlarmHandlerForTimeout@ha
    addi r7, r3, AlarmHandlerForTimeout@l
    addi r3, r31, 0x0
    addi r6, r30, 0x0
    li r5, 0x0
    bl OSSetAlarm
    lwz r0, 0x1c(r1)
    li r3, 0x1
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
