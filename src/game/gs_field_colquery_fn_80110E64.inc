    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stmw r17, 0x64(r1)
    mr r27, r5
    fmuls f31, f1, f1
    mr r30, r8
    lwz r17, 0x0(r5)
    mr r25, r3
    mr r26, r4
    mr r28, r6
    mr r29, r7
    mr r22, r30
    li r31, 0x0
    li r18, 0x0
    b @L_8011103C
@L_80110EAC:
    mr r4, r30
    li r5, 0x0
    mtctr r31
    cmpwi r31, 0x0
    ble @L_80110EDC
@L_80110EC0:
    lhz r3, 0x32(r17)
    lhz r0, 0x30(r4)
    cmplw r3, r0
    beq @L_80110EDC
    addi r4, r4, 0x34
    addi r5, r5, 0x1
    bdnz @L_80110EC0
@L_80110EDC:
    cmpw r5, r31
    blt @L_80111034
    mr r3, r29
    addi r4, r17, 0x24
    addi r5, r1, 0x30
    bl fn_800A37CC
    mr r4, r26
    addi r3, r1, 0x30
    bl fn_800A3B7C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_80111034
    addi r20, r1, 0x3c
    mr r19, r17
    li r21, 0x0
@L_80110F1C:
    mr r3, r28
    mr r4, r19
    mr r5, r20
    bl fn_800A37CC
    addi r21, r21, 0x1
    addi r19, r19, 0xc
    cmpwi r21, 0x3
    addi r20, r20, 0xc
    blt @L_80110F1C
    mr r5, r25
    addi r3, r1, 0x30
    addi r4, r1, 0x3c
    bl fn_8010C77C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    bge @L_80110F64
    li r0, 0x0
    b @L_80110FBC
@L_80110F64:
    mr r6, r25
    addi r3, r1, 0x24
    addi r4, r1, 0x30
    addi r5, r1, 0x3c
    bl fn_8010FA54
    mr r4, r25
    addi r3, r1, 0x24
    bl fn_800A3BD8
    fcmpo cr0, f1, f31
    cror eq, gt, eq
    bne @L_80110F98
    li r0, 0x0
    b @L_80110FBC
@L_80110F98:
    addi r3, r1, 0x24
    addi r4, r1, 0x3c
    addi r5, r1, 0x30
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_80110FB8
    li r0, 0x0
    b @L_80110FBC
@L_80110FB8:
    li r0, 0x1
@L_80110FBC:
    cmpwi r0, 0x0
    beq @L_80111034
    lhz r4, 0x32(r17)
    addi r31, r31, 0x1
    lwz r3, 0x3c(r1)
    lwz r0, 0x40(r1)
    stw r3, 0x0(r22)
    stw r0, 0x4(r22)
    lwz r0, 0x44(r1)
    stw r0, 0x8(r22)
    lwz r3, 0x48(r1)
    lwz r0, 0x4c(r1)
    stw r3, 0xc(r22)
    stw r0, 0x10(r22)
    lwz r0, 0x50(r1)
    stw r0, 0x14(r22)
    lwz r3, 0x54(r1)
    lwz r0, 0x58(r1)
    stw r3, 0x18(r22)
    stw r0, 0x1c(r22)
    lwz r0, 0x5c(r1)
    stw r0, 0x20(r22)
    lwz r3, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r3, 0x24(r22)
    stw r0, 0x28(r22)
    lwz r0, 0x38(r1)
    stw r0, 0x2c(r22)
    sth r4, 0x30(r22)
    addi r22, r22, 0x34
@L_80111034:
    addi r18, r18, 0x1
    addi r17, r17, 0x34
@L_8011103C:
    lwz r0, 0x4(r27)
    cmplw r18, r0
    bge @L_80111050
    cmpwi r31, 0x4
    blt @L_80110EAC
@L_80111050:
    mulli r0, r31, 0x34
    lwz r18, 0x0(r27)
    li r17, 0x0
    add r21, r30, r0
    b @L_8011124C
@L_80111064:
    lhz r0, 0x30(r18)
    rlwinm. r0, r0, 0, 29, 31
    beq @L_80111244
    mr r4, r30
    li r5, 0x0
    mtctr r31
    cmpwi r31, 0x0
    ble @L_801110A0
@L_80111084:
    lhz r3, 0x32(r18)
    lhz r0, 0x30(r4)
    cmplw r3, r0
    beq @L_801110A0
    addi r4, r4, 0x34
    addi r5, r5, 0x1
    bdnz @L_80111084
@L_801110A0:
    cmpw r5, r31
    blt @L_80111244
    mr r3, r29
    addi r4, r18, 0x24
    addi r5, r1, 0x30
    bl fn_800A37CC
    mr r4, r26
    addi r3, r1, 0x30
    bl fn_800A3B7C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_80111244
    addi r20, r1, 0x3c
    mr r22, r18
    mr r19, r20
    li r23, 0x0
@L_801110E4:
    mr r3, r28
    mr r4, r22
    mr r5, r19
    bl fn_800A37CC
    addi r23, r23, 0x1
    addi r22, r22, 0xc
    cmpwi r23, 0x3
    addi r19, r19, 0xc
    blt @L_801110E4
    lwz r4, -26456(r2)
    mr r5, r25
    lhz r0, -26452(r2)
    addi r3, r1, 0x30
    stw r4, 0x10(r1)
    addi r4, r1, 0x3c
    lhz r23, 0x30(r18)
    sth r0, 0x14(r1)
    bl fn_8010C77C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    bge @L_80111140
    li r0, 0x0
    b @L_801111CC
@L_80111140:
    addi r22, r1, 0x10
    li r24, 0x0
@L_80111148:
    lhz r0, 0x0(r22)
    and. r0, r23, r0
    beq @L_801111B4
    addi r0, r24, 0x1
    cmpwi r0, 0x3
    blt @L_80111164
    li r0, 0x0
@L_80111164:
    mulli r0, r0, 0xc
    addi r5, r1, 0x3c
    mr r4, r20
    mr r6, r25
    addi r3, r1, 0x18
    add r5, r5, r0
    bl fn_8010F5A4
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    blt @L_801111B4
    lfs f0, -26436(r2)
    fcmpo cr0, f1, f0
    bgt @L_801111B4
    mr r4, r25
    addi r3, r1, 0x18
    bl fn_800A3BD8
    fcmpo cr0, f1, f31
    bge @L_801111B4
    li r0, 0x1
    b @L_801111CC
@L_801111B4:
    addi r24, r24, 0x1
    addi r20, r20, 0xc
    cmpwi r24, 0x3
    addi r22, r22, 0x2
    blt @L_80111148
    li r0, 0x0
@L_801111CC:
    cmpwi r0, 0x0
    beq @L_80111244
    lhz r4, 0x32(r18)
    addi r31, r31, 0x1
    lwz r3, 0x3c(r1)
    lwz r0, 0x40(r1)
    stw r3, 0x0(r21)
    stw r0, 0x4(r21)
    lwz r0, 0x44(r1)
    stw r0, 0x8(r21)
    lwz r3, 0x48(r1)
    lwz r0, 0x4c(r1)
    stw r3, 0xc(r21)
    stw r0, 0x10(r21)
    lwz r0, 0x50(r1)
    stw r0, 0x14(r21)
    lwz r3, 0x54(r1)
    lwz r0, 0x58(r1)
    stw r3, 0x18(r21)
    stw r0, 0x1c(r21)
    lwz r0, 0x5c(r1)
    stw r0, 0x20(r21)
    lwz r3, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r3, 0x24(r21)
    stw r0, 0x28(r21)
    lwz r0, 0x38(r1)
    stw r0, 0x2c(r21)
    sth r4, 0x30(r21)
    addi r21, r21, 0x34
@L_80111244:
    addi r17, r17, 0x1
    addi r18, r18, 0x34
@L_8011124C:
    lwz r0, 0x4(r27)
    cmplw r17, r0
    bge @L_80111260
    cmpwi r31, 0x4
    blt @L_80111064
@L_80111260:
    mulli r0, r31, 0x34
    lwz r18, 0x0(r27)
    li r17, 0x0
    add r20, r30, r0
    b @L_8011143C
@L_80111274:
    lhz r0, 0x30(r18)
    rlwinm. r0, r0, 0, 29, 31
    beq @L_80111434
    mr r4, r30
    li r5, 0x0
    mtctr r31
    cmpwi r31, 0x0
    ble @L_801112B0
@L_80111294:
    lhz r3, 0x32(r18)
    lhz r0, 0x30(r4)
    cmplw r3, r0
    beq @L_801112B0
    addi r4, r4, 0x34
    addi r5, r5, 0x1
    bdnz @L_80111294
@L_801112B0:
    cmpw r5, r31
    blt @L_80111434
    mr r3, r29
    addi r4, r18, 0x24
    addi r5, r1, 0x30
    bl fn_800A37CC
    mr r4, r26
    addi r3, r1, 0x30
    bl fn_800A3B7C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_80111434
    addi r19, r1, 0x3c
    mr r22, r18
    mr r21, r19
    li r23, 0x0
@L_801112F4:
    mr r3, r28
    mr r4, r22
    mr r5, r21
    bl fn_800A37CC
    addi r23, r23, 0x1
    addi r22, r22, 0xc
    cmpwi r23, 0x3
    addi r21, r21, 0xc
    blt @L_801112F4
    lwz r4, -26448(r2)
    mr r5, r25
    lhz r0, -26444(r2)
    addi r3, r1, 0x30
    stw r4, 0x8(r1)
    addi r4, r1, 0x3c
    lhz r22, 0x30(r18)
    sth r0, 0xc(r1)
    bl fn_8010C77C
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    bge @L_80111350
    li r0, 0x0
    b @L_801113BC
@L_80111350:
    addi r21, r1, 0x8
    li r23, 0x0
    mr r24, r21
@L_8011135C:
    addi r3, r23, 0x2
    cmpwi r3, 0x3
    blt @L_8011136C
    subi r3, r3, 0x3
@L_8011136C:
    lhz r0, 0x0(r21)
    and. r0, r22, r0
    beq @L_801113A4
    rlwinm r0, r3, 1, 0, 30
    lhzx r0, r24, r0
    and. r0, r22, r0
    beq @L_801113A4
    mr r3, r19
    mr r4, r25
    bl fn_800A3BD8
    fcmpo cr0, f1, f31
    bge @L_801113A4
    li r0, 0x1
    b @L_801113BC
@L_801113A4:
    addi r23, r23, 0x1
    addi r19, r19, 0xc
    cmpwi r23, 0x3
    addi r21, r21, 0x2
    blt @L_8011135C
    li r0, 0x0
@L_801113BC:
    cmpwi r0, 0x0
    beq @L_80111434
    lhz r4, 0x32(r18)
    addi r31, r31, 0x1
    lwz r3, 0x3c(r1)
    lwz r0, 0x40(r1)
    stw r3, 0x0(r20)
    stw r0, 0x4(r20)
    lwz r0, 0x44(r1)
    stw r0, 0x8(r20)
    lwz r3, 0x48(r1)
    lwz r0, 0x4c(r1)
    stw r3, 0xc(r20)
    stw r0, 0x10(r20)
    lwz r0, 0x50(r1)
    stw r0, 0x14(r20)
    lwz r3, 0x54(r1)
    lwz r0, 0x58(r1)
    stw r3, 0x18(r20)
    stw r0, 0x1c(r20)
    lwz r0, 0x5c(r1)
    stw r0, 0x20(r20)
    lwz r3, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r3, 0x24(r20)
    stw r0, 0x28(r20)
    lwz r0, 0x38(r1)
    stw r0, 0x2c(r20)
    sth r4, 0x30(r20)
    addi r20, r20, 0x34
@L_80111434:
    addi r17, r17, 0x1
    addi r18, r18, 0x34
@L_8011143C:
    lwz r0, 0x4(r27)
    cmplw r17, r0
    bge @L_80111450
    cmpwi r31, 0x4
    blt @L_80111274
@L_80111450:
    mr r3, r31
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    lmw r17, 0x64(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr

