    nofralloc
    stwu r1, -0x170(r1)
    mflr r0
    stw r0, 0x174(r1)
    stfd f31, 0x168(r1)
    fmr f31, f1
    stmw r23, 0x144(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    li r28, 0x0
    li r24, 0x0
    bl fn_8010CBC0
    mr r31, r3
    li r29, 0x0
    lwz r30, 0x0(r3)
    b @L_8011160C
@L_801114B0:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8010C7BC
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_80111604
    lwz r23, 0x2c(r30)
    cmplwi r23, 0x0
    beq @L_80111604
    lhz r0, 0x3c(r30)
    clrlwi. r0, r0, 31
    beq @L_8011151C
    mr r4, r29
    addi r3, r1, 0x3c
    bl fn_8010CA30
    mr r4, r29
    addi r3, r1, 0xc
    bl fn_8010C8D0
    fmr f1, f31
    mr r3, r25
    mr r4, r26
    mr r5, r23
    addi r6, r1, 0x3c
    addi r7, r1, 0xc
    addi r8, r1, 0x6c
    bl fn_80110E64
    b @L_80111534
@L_8011151C:
    fmr f1, f31
    mr r3, r25
    mr r4, r26
    mr r5, r23
    addi r6, r1, 0x6c
    bl fn_8011069C
@L_80111534:
    addi r4, r1, 0x6c
    li r5, 0x0
    b @L_801115F4
@L_80111540:
    mr r7, r27
    li r8, 0x0
    mtctr r28
    cmpwi r28, 0x0
    ble @L_80111570
@L_80111554:
    lhz r6, 0x30(r7)
    lhz r0, 0x30(r4)
    cmplw r6, r0
    beq @L_80111570
    addi r7, r7, 0x34
    addi r8, r8, 0x1
    bdnz @L_80111554
@L_80111570:
    cmpw r8, r28
    blt @L_801115EC
    lwz r6, 0x0(r4)
    add r7, r27, r24
    lwz r0, 0x4(r4)
    addi r28, r28, 0x1
    addi r24, r24, 0x34
    stw r6, 0x0(r7)
    stw r0, 0x4(r7)
    lwz r6, 0x8(r4)
    lwz r0, 0xc(r4)
    stw r6, 0x8(r7)
    stw r0, 0xc(r7)
    lwz r6, 0x10(r4)
    lwz r0, 0x14(r4)
    stw r6, 0x10(r7)
    stw r0, 0x14(r7)
    lwz r6, 0x18(r4)
    lwz r0, 0x1c(r4)
    stw r6, 0x18(r7)
    stw r0, 0x1c(r7)
    lwz r6, 0x20(r4)
    lwz r0, 0x24(r4)
    stw r6, 0x20(r7)
    stw r0, 0x24(r7)
    lwz r6, 0x28(r4)
    lwz r0, 0x2c(r4)
    stw r6, 0x28(r7)
    stw r0, 0x2c(r7)
    lwz r0, 0x30(r4)
    stw r0, 0x30(r7)
@L_801115EC:
    addi r4, r4, 0x34
    addi r5, r5, 0x1
@L_801115F4:
    cmpw r5, r3
    bge @L_80111604
    cmpwi r28, 0x4
    blt @L_80111540
@L_80111604:
    addi r29, r29, 0x1
    addi r30, r30, 0x40
@L_8011160C:
    lwz r0, 0x4(r31)
    cmplw r29, r0
    bge @L_80111620
    cmpwi r28, 0x4
    blt @L_801114B0
@L_80111620:
    mr r3, r28
    lfd f31, 0x168(r1)
    lmw r23, 0x144(r1)
    lwz r0, 0x174(r1)
    mtlr r0
    addi r1, r1, 0x170
    blr
