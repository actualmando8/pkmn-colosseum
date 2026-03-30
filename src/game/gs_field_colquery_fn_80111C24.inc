    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stmw r21, 0xc4(r1)
    mr r21, r3
    mr r22, r4
    bl fn_8010CBC0
    cmplwi r3, 0x0
    bne @L_80111C50
    li r3, 0x0
    b @L_80111DE4
@L_80111C50:
    mr r3, r22
    mr r4, r21
    bl fn_800A3C00
    lfs f0, -26424(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80111C74
    li r3, 0x0
    b @L_80111DE4
@L_80111C74:
    bl fn_8010CBC0
    mr r31, r3
    mr r3, r22
    mr r4, r21
    addi r5, r1, 0x28
    bl fn_800A3A9C
    lwz r28, 0x0(r31)
    li r30, 0x0
    b @L_80111DD4
@L_80111C98:
    mr r3, r30
    addi r4, r1, 0xc
    bl fn_8010C7BC
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    beq @L_80111DCC
    lwz r29, 0x38(r28)
    cmplwi r29, 0x0
    beq @L_80111DCC
    mr r4, r30
    addi r3, r1, 0x58
    bl fn_8010CA30
    mr r4, r30
    addi r3, r1, 0x88
    bl fn_8010C8D0
    lwz r25, 0x0(r29)
    li r26, 0x0
    b @L_80111DAC
@L_80111CE0:
    addi r3, r1, 0x88
    addi r4, r25, 0x24
    addi r5, r1, 0x1c
    bl fn_800A37CC
    mr r23, r25
    addi r24, r1, 0x34
    li r27, 0x0
@L_80111CFC:
    mr r4, r23
    mr r5, r24
    addi r3, r1, 0x58
    bl fn_800A37CC
    addi r27, r27, 0x1
    addi r23, r23, 0xc
    cmpwi r27, 0x3
    addi r24, r24, 0xc
    blt @L_80111CFC
    mr r7, r21
    mr r8, r22
    addi r3, r1, 0x10
    addi r4, r1, 0x8
    addi r5, r1, 0x1c
    addi r6, r1, 0x34
    bl fn_8010F4B8
    cmpwi r3, 0x0
    bne @L_80111D4C
    li r0, 0x0
    b @L_80111D94
@L_80111D4C:
    lfs f1, 0x8(r1)
    lfs f0, -26424(r2)
    fcmpo cr0, f1, f0
    blt @L_80111D68
    lfs f0, -26420(r2)
    fcmpo cr0, f1, f0
    ble @L_80111D70
@L_80111D68:
    li r0, 0x0
    b @L_80111D94
@L_80111D70:
    addi r3, r1, 0x10
    addi r4, r1, 0x34
    addi r5, r1, 0x1c
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_80111D90
    li r0, 0x0
    b @L_80111D94
@L_80111D90:
    li r0, 0x1
@L_80111D94:
    cmpwi r0, 0x0
    beq @L_80111DA4
    li r0, 0x1
    b @L_80111DBC
@L_80111DA4:
    addi r26, r26, 0x1
    addi r25, r25, 0x30
@L_80111DAC:
    lwz r0, 0x4(r29)
    cmplw r26, r0
    blt @L_80111CE0
    li r0, 0x0
@L_80111DBC:
    cmpwi r0, 0x0
    beq @L_80111DCC
    li r3, 0x1
    b @L_80111DE4
@L_80111DCC:
    addi r30, r30, 0x1
    addi r28, r28, 0x40
@L_80111DD4:
    lwz r0, 0x4(r31)
    cmplw r30, r0
    blt @L_80111C98
    li r3, 0x0
@L_80111DE4:
    lmw r21, 0xc4(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr

