    nofralloc
    mflr r0
    lis r3, lbl_803FC620@ha
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, lbl_803FC620@l
    stw r29, 0xc(r1)
    lwz r0, 0x10c(r30)
    cmplwi r0, 0x0
    beq @L_800B00F8
    lwz r0, 0x21c(r30)
    cmplwi r0, 0x0
    bne @L_800B0150
    @L_800B00F8:
    bl fn_8009D820
    sth r3, lbl_8047A970(r13)
    lwz r3, lbl_80478A48(r13)
    bl OSRegisterVersion
    bl fn_800AE7E0
    bl OSInitAlarm
    li r29, 0x0
    li r31, -0x3
    @L_800B0118:
    stw r31, 0x4(r30)
    addi r3, r30, 0x8c
    bl OSInitThreadQueue
    addi r3, r30, 0xe0
    bl OSCreateAlarm
    addi r29, r29, 0x1
    cmpwi r29, 0x2
    addi r30, r30, 0x110
    blt @L_800B0118
    lis r3, 0x8000
    bl fn_800B0174
    lis r3, lbl_803127F0@ha
    addi r3, r3, lbl_803127F0@l
    bl OSRegisterResetFunction
    @L_800B0150:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    lwz r29, 0xc(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
