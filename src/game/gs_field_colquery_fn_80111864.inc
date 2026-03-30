    nofralloc
    stwu r1, -0x1d0(r1)
    mflr r0
    stw r0, 0x1d4(r1)
    stmw r15, 0x18c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    li r31, 0x0
    li r27, 0x0
    bl fn_8010CBC0
    mr r26, r3
    mr r3, r29
    mr r4, r28
    addi r5, r1, 0x28
    bl fn_800A3A9C
    lwz r19, 0x0(r26)
    li r17, 0x0
    b @L_80111B70
@L_801118AC:
    mr r3, r17
    addi r4, r1, 0xc
    bl fn_8010C7BC
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    beq @L_80111B68
    lwz r18, 0x30(r19)
    cmplwi r18, 0x0
    beq @L_80111B68
    mr r4, r17
    addi r3, r1, 0x88
    bl fn_8010CA30
    mr r4, r17
    addi r3, r1, 0x58
    bl fn_8010C8D0
    addi r21, r1, 0xb8
    li r25, 0x0
    lwz r23, 0x0(r18)
    mr r24, r25
    mr r22, r21
    b @L_80111A88
@L_80111900:
    mr r5, r21
    li r4, 0x0
    mtctr r25
    cmpwi r25, 0x0
    ble @L_80111930
@L_80111914:
    lhz r3, 0x30(r23)
    lhz r0, 0x30(r5)
    cmplw r3, r0
    beq @L_80111930
    addi r5, r5, 0x34
    addi r4, r4, 0x1
    bdnz @L_80111914
@L_80111930:
    cmpw r4, r25
    blt @L_80111A80
    addi r3, r1, 0x58
    addi r4, r23, 0x24
    addi r5, r1, 0x1c
    bl fn_800A37CC
    addi r3, r1, 0x1c
    addi r4, r1, 0x28
    bl fn_800A3B7C
    lfs f0, -26432(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_80111A80
    addi r15, r1, 0x34
    mr r16, r23
    li r20, 0x0
@L_80111970:
    mr r4, r16
    mr r5, r15
    addi r3, r1, 0x88
    bl fn_800A37CC
    addi r20, r20, 0x1
    addi r16, r16, 0xc
    cmpwi r20, 0x3
    addi r15, r15, 0xc
    blt @L_80111970
    mr r7, r28
    mr r8, r29
    addi r3, r1, 0x10
    addi r4, r1, 0x8
    addi r5, r1, 0x1c
    addi r6, r1, 0x34
    bl fn_8010F4B8
    cmpwi r3, 0x0
    bne @L_801119C0
    li r0, 0x0
    b @L_80111A08
@L_801119C0:
    lfs f1, 0x8(r1)
    lfs f0, -26432(r2)
    fcmpo cr0, f1, f0
    blt @L_801119DC
    lfs f0, -26428(r2)
    fcmpo cr0, f1, f0
    ble @L_801119E4
@L_801119DC:
    li r0, 0x0
    b @L_80111A08
@L_801119E4:
    addi r3, r1, 0x10
    addi r4, r1, 0x34
    addi r5, r1, 0x1c
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_80111A04
    li r0, 0x0
    b @L_80111A08
@L_80111A04:
    li r0, 0x1
@L_80111A08:
    cmpwi r0, 0x0
    beq @L_80111A80
    lwz r3, 0x34(r1)
    addi r25, r25, 0x1
    lwz r0, 0x38(r1)
    stw r3, 0x0(r22)
    lwz r3, 0x3c(r1)
    stw r0, 0x4(r22)
    lwz r0, 0x40(r1)
    stw r3, 0x8(r22)
    lwz r3, 0x44(r1)
    stw r0, 0xc(r22)
    lwz r0, 0x48(r1)
    stw r3, 0x10(r22)
    lwz r3, 0x4c(r1)
    stw r0, 0x14(r22)
    lwz r0, 0x50(r1)
    stw r3, 0x18(r22)
    lwz r3, 0x54(r1)
    stw r0, 0x1c(r22)
    lwz r0, 0x1c(r1)
    stw r3, 0x20(r22)
    lwz r3, 0x20(r1)
    stw r0, 0x24(r22)
    lwz r0, 0x24(r1)
    stw r3, 0x28(r22)
    lhz r3, 0x30(r23)
    stw r0, 0x2c(r22)
    sth r3, 0x30(r22)
    addi r22, r22, 0x34
@L_80111A80:
    addi r24, r24, 0x1
    addi r23, r23, 0x34
@L_80111A88:
    lwz r0, 0x4(r18)
    cmplw r24, r0
    bge @L_80111A9C
    cmpwi r25, 0x4
    blt @L_80111900
@L_80111A9C:
    li r3, 0x0
    b @L_80111B58
@L_80111AA4:
    mr r5, r30
    li r6, 0x0
    mtctr r31
    cmpwi r31, 0x0
    ble @L_80111AD4
@L_80111AB8:
    lhz r4, 0x30(r5)
    lhz r0, 0x30(r21)
    cmplw r4, r0
    beq @L_80111AD4
    addi r5, r5, 0x34
    addi r6, r6, 0x1
    bdnz @L_80111AB8
@L_80111AD4:
    cmpw r6, r31
    blt @L_80111B50
    lwz r0, 0x0(r21)
    add r5, r30, r27
    lwz r4, 0x4(r21)
    addi r31, r31, 0x1
    stw r0, 0x0(r5)
    addi r27, r27, 0x34
    lwz r0, 0x8(r21)
    stw r4, 0x4(r5)
    lwz r4, 0xc(r21)
    stw r0, 0x8(r5)
    lwz r0, 0x10(r21)
    stw r4, 0xc(r5)
    lwz r4, 0x14(r21)
    stw r0, 0x10(r5)
    lwz r0, 0x18(r21)
    stw r4, 0x14(r5)
    lwz r4, 0x1c(r21)
    stw r0, 0x18(r5)
    lwz r0, 0x20(r21)
    stw r4, 0x1c(r5)
    lwz r4, 0x24(r21)
    stw r0, 0x20(r5)
    lwz r0, 0x28(r21)
    stw r4, 0x24(r5)
    lwz r4, 0x2c(r21)
    stw r0, 0x28(r5)
    lwz r0, 0x30(r21)
    stw r4, 0x2c(r5)
    stw r0, 0x30(r5)
@L_80111B50:
    addi r21, r21, 0x34
    addi r3, r3, 0x1
@L_80111B58:
    cmpw r3, r25
    bge @L_80111B68
    cmpwi r31, 0x4
    blt @L_80111AA4
@L_80111B68:
    addi r17, r17, 0x1
    addi r19, r19, 0x40
@L_80111B70:
    lwz r0, 0x4(r26)
    cmplw r17, r0
    bge @L_80111B84
    cmpwi r31, 0x4
    blt @L_801118AC
@L_80111B84:
    mr r3, r31
    lmw r15, 0x18c(r1)
    lwz r0, 0x1d4(r1)
    mtlr r0
    addi r1, r1, 0x1d0
    blr

