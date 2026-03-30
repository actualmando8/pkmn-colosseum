    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    bl fn_800A13F8
    lis r4, -32709
    stw r3, -25120(r13)
    addi r3, r4, 28128
    bl OSCreateAlarm
    bl OSDisableInterrupts
    lis r4, 0x8000
    lis r5, 0x1062
    lwz r0, 0xf8(r4)
    lis r4, -32761
    addi r5, r5, 0x4dd3
    lis r6, -32709
    rlwinm r0, r0, 30, 2, 31
    addi r7, r4, 9860
    mulhwu r0, r5, r0
    addi r5, r6, 28128
    mr r30, r3
    mr r3, r5
    li r5, 0x0
    rlwinm r6, r0, 26, 6, 31
    bl OSSetAlarm
    lwz r3, -25120(r13)
    bl fn_800A221C
    mr r3, r30
    bl OSRestoreInterrupts
    mr r3, r31
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_80071A40
    li r30, 0x1
    b @L_80071AAC
@L_80071A40:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80071A5C
    li r30, 0x2
    b @L_80071AAC
@L_80071A5C:
    lbz r0, 0x8(r1)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    bne @L_80071A74
    li r30, -0x1
    b @L_80071AAC
@L_80071A74:
    mr r3, r31
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80071A94
    li r30, 0x3
    b @L_80071AAC
@L_80071A94:
    lwz r0, 0xc(r1)
    cmplwi r0, 0x0
    beq @L_80071AA8
    li r30, 0x4
    b @L_80071AAC
@L_80071AA8:
    li r30, 0x0
@L_80071AAC:
    cmpwi r30, 0x0
    beq @L_80071ABC
    cmpwi r30, 0x3
    blt @L_80071AC8
@L_80071ABC:
    addi r3, r31, 0x1
    li r4, 0x1
    bl fn_8008ABE4
@L_80071AC8:
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
