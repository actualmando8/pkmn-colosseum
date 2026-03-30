    nofralloc
    mflr r0
    li r6, 0x20
    stw r0, 0x4(r1)
    li r0, 0x0
    lis r5, 0x8000
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    stw r4, Callback_8047A788(r13)
    lis r4, 0xcc00
    addi r7, r4, 0x6000
    stw r0, StopAtNextInt_8047A780(r13)
    lis r0, 0x1200
    stw r0, 0x6008(r4)
    lis r4, AlarmForTimeout_803FC2F8@ha
    li r0, 0x3
    stw r6, 0x10(r7)
    addi r31, r4, AlarmForTimeout_803FC2F8@l
    stw r3, 0x14(r7)
    addi r3, r31, 0x0
    stw r6, 0x18(r7)
    stw r0, 0x1c(r7)
    lwz r0, 0xf8(r5)
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
