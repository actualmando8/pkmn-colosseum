    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    stw r31, 0xfc(r1)
    stw r30, 0xf8(r1)
    stw r29, 0xf4(r1)
    stw r28, 0xf0(r1)
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0388
    lwz r3, -21580(r13)
    li r31, 0x0
    lwz r29, 0x14(r3)
    addi r28, r3, 0x8c
@L_801CFD48:
    mr r3, r29
    mr r4, r31
    mr r5, r28
    bl fn_800B4488
    mr. r30, r3
    bne @L_801CFE74
    mr r3, r29
    mr r4, r31
    addi r5, r1, 0x10
    bl fn_800B5530
    mr. r30, r3
    bne @L_801CFE60
    bl fn_800057A0
    cmpwi r3, 0x1
    beq @L_801CFDE0
    bge @L_801CFD94
    cmpwi r3, 0x0
    bge @L_801CFDA0
    b @L_801CFE60
@L_801CFD94:
    cmpwi r3, 0x3
    bge @L_801CFE60
    b @L_801CFE20
@L_801CFDA0:
    lis r4, -32728
    addi r3, r1, 0x10
    addi r4, r4, -27928
    bl fn_800CA7FC
    cmpwi r3, 0x0
    bne @L_801CFE60
    lwz r3, 0x38(r1)
    lwz r0, -21816(r2)
    cmplw r3, r0
    bne @L_801CFE60
    lhz r3, 0x3c(r1)
    lhz r0, -21808(r2)
    cmplw r3, r0
    bne @L_801CFE60
    li r0, 0x1
    b @L_801CFE64
@L_801CFDE0:
    lis r4, -32728
    addi r3, r1, 0x10
    addi r4, r4, -27928
    bl fn_800CA7FC
    cmpwi r3, 0x0
    bne @L_801CFE60
    lwz r3, 0x38(r1)
    lwz r0, -21804(r2)
    cmplw r3, r0
    bne @L_801CFE60
    lhz r3, 0x3c(r1)
    lhz r0, -21808(r2)
    cmplw r3, r0
    bne @L_801CFE60
    li r0, 0x1
    b @L_801CFE64
@L_801CFE20:
    lis r4, -32728
    addi r3, r1, 0x10
    addi r4, r4, -27928
    bl fn_800CA7FC
    cmpwi r3, 0x0
    bne @L_801CFE60
    lwz r3, 0x38(r1)
    lwz r0, -21804(r2)
    cmplw r3, r0
    bne @L_801CFE60
    lhz r3, 0x3c(r1)
    lhz r0, -21808(r2)
    cmplw r3, r0
    bne @L_801CFE60
    li r0, 0x1
    b @L_801CFE64
@L_801CFE60:
    li r0, 0x0
@L_801CFE64:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801CFE74
    li r30, 0x0
    b @L_801CFE8C
@L_801CFE74:
    addi r31, r31, 0x1
    cmpwi r31, 0x7f
    blt @L_801CFD48
    cmpwi r30, 0x0
    bne @L_801CFE8C
    li r30, -0x4
@L_801CFE8C:
    cmpwi r30, 0x0
    mr r5, r30
    beq @L_801CFEA8
    bge @L_801D0054
    cmpwi r30, -0x4
    beq @L_801CFFD0
    b @L_801D0054
@L_801CFEA8:
    lwz r3, -21580(r13)
    li r0, 0x1
    stb r0, 0x3e(r3)
    bl fn_800057A8
    cmpwi r3, 0x4
    bne @L_801CFF44
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B01AC
    lwz r4, -21580(r13)
    stw r3, 0x68(r4)
    lwz r5, -21580(r13)
    lwz r4, 0x68(r5)
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x6c(r5)
    stw r0, 0x70(r5)
    lwz r3, 0x8(r4)
    lwz r0, 0xc(r4)
    stw r3, 0x74(r5)
    stw r0, 0x78(r5)
    lwz r3, 0x10(r4)
    lwz r0, 0x14(r4)
    stw r3, 0x7c(r5)
    stw r0, 0x80(r5)
    lwz r3, 0x18(r4)
    lwz r0, 0x1c(r4)
    stw r3, 0x84(r5)
    stw r0, 0x88(r5)
    lwz r0, -21816(r2)
    lwz r3, -21580(r13)
    stw r0, 0x6c(r3)
    lhz r0, -21808(r2)
    lwz r3, -21580(r13)
    sth r0, 0x70(r3)
    lwz r4, -21580(r13)
    lwz r3, 0x14(r4)
    addi r4, r4, 0x6c
    bl fn_800B01C4
@L_801CFF44:
    lwz r4, -21580(r13)
    addi r5, r1, 0x7c
    lwz r3, 0x14(r4)
    lwz r4, 0x90(r4)
    bl fn_800B5530
    mr. r5, r3
    bne @L_801D0054
    lwz r3, 0xb4(r1)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801CFFB4
    lwz r4, -21580(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0xc
    bne @L_801CFF9C
    li r3, 0x2
    li r0, 0xb
    stw r3, 0x4(r4)
    li r3, 0x30
    lwz r4, -21580(r13)
    stw r0, 0x10(r4)
    b @L_801D0060
@L_801CFF9C:
    lwz r0, 0x0(r4)
    li r3, 0x19
    cmpwi r0, 0x3
    bne @L_801D0060
    li r3, 0x1c
    b @L_801D0060
@L_801CFFB4:
    lwz r4, 0xc(r1)
    li r5, -0x4
    lwz r3, 0x8(r1)
    addis r4, r4, 0x6
    addi r0, r3, 0x1
    stw r4, 0xc(r1)
    stw r0, 0x8(r1)
@L_801CFFD0:
    lwz r4, -21580(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x9
    beq @L_801D0038
    bge @L_801D0054
    cmpwi r0, 0x1
    beq @L_801CFFF0
    b @L_801D0054
@L_801CFFF0:
    lwz r3, 0xc(r1)
    lis r0, 0x6
    cmpw r3, r0
    bge @L_801D0008
    li r5, -0x9
    b @L_801D0054
@L_801D0008:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x1
    bge @L_801D001C
    li r5, -0x8
    b @L_801D0054
@L_801D001C:
    li r3, 0x6
    li r0, 0xa
    stw r3, 0x4(r4)
    li r3, 0x30
    lwz r4, -21580(r13)
    stw r0, 0x10(r4)
    b @L_801D0060
@L_801D0038:
    li r3, 0x2
    li r0, 0xb
    stw r3, 0x4(r4)
    li r3, 0x30
    lwz r4, -21580(r13)
    stw r0, 0x10(r4)
    b @L_801D0060
@L_801D0054:
    lwz r4, -21580(r13)
    li r3, 0x2b
    stw r5, 0x4(r4)
@L_801D0060:
    lwz r0, 0x104(r1)
    lwz r31, 0xfc(r1)
    lwz r30, 0xf8(r1)
    lwz r29, 0xf4(r1)
    lwz r28, 0xf0(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
