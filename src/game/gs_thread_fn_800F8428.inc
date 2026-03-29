    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_80401C10@ha
    li r4, 0x4
    addi r3, r3, lbl_80401C10@l
    stw r0, 0x14(r1)
    li r0, 0x0
    li r7, 0x0
    addi r5, r3, 0x1c0
    addi r6, r3, 0x1b0
    mtctr r4
@L_800F8454:
    addi r4, r3, 0x0
    addi r10, r7, 0x1
    lwz r8, 0x0(r4)
    li r9, 0x0
    cmpw r8, r10
    bne @L_800F8470
    b @L_800F84A4
@L_800F8470:
    lwzu r8, 0x6c(r4)
    cmpw r8, r10
    bne @L_800F8480
    b @L_800F84A4
@L_800F8480:
    lwzu r8, 0x6c(r4)
    cmpw r8, r10
    bne @L_800F8490
    b @L_800F84A4
@L_800F8490:
    lwzu r8, 0x6c(r4)
    cmpw r8, r10
    bne @L_800F84A0
    b @L_800F84A4
@L_800F84A0:
    mr r4, r9
@L_800F84A4:
    cmplwi r4, 0x0
    beq @L_800F8624
    lwz r8, 0xc(r4)
    cmpwi r8, 0x0
    bne @L_800F8624
    lwz r8, 0x5c(r4)
    cmpwi r8, 0x2
    beq @L_800F858C
    bge @L_800F84D4
    cmpwi r8, 0x1
    bge @L_800F84E0
    b @L_800F85C0
@L_800F84D4:
    cmpwi r8, 0x4
    bge @L_800F85C0
    b @L_800F85A8
@L_800F84E0:
    lwz r8, 0x60(r4)
    cmplwi r8, 0xe10
    ble @L_800F8508
    lwz r8, 0x0(r5)
    cmplwi r8, 0x1
    beq @L_800F8508
    li r8, 0x1
    li r0, 0x1
    stw r8, 0x0(r5)
    b @L_800F85C0
@L_800F8508:
    lwz r8, 0x60(r4)
    cmplwi r8, 0x64
    bge @L_800F8530
    lwz r8, 0x0(r5)
    cmplwi r8, 0x0
    beq @L_800F8530
    li r8, 0x0
    li r0, 0x1
    stw r8, 0x0(r5)
    b @L_800F85C0
@L_800F8530:
    lwz r9, 0x0(r6)
    lwz r8, 0x60(r4)
    add r8, r9, r8
    stw r8, 0x0(r6)
    lwz r8, 0x0(r6)
    cmpwi r8, 0xe10
    ble @L_800F8570
    li r8, 0x0
    stw r8, 0x0(r6)
    lwz r8, 0x0(r5)
    cmplwi r8, 0x1
    beq @L_800F85C0
    li r8, 0x1
    li r0, 0x1
    stw r8, 0x0(r5)
    b @L_800F85C0
@L_800F8570:
    lwz r8, 0x0(r5)
    cmplwi r8, 0x0
    beq @L_800F85C0
    li r8, 0x0
    li r0, 0x1
    stw r8, 0x0(r5)
    b @L_800F85C0
@L_800F858C:
    lwz r8, 0x0(r5)
    cmplwi r8, 0x0
    beq @L_800F85C0
    li r8, 0x0
    li r0, 0x1
    stw r8, 0x0(r5)
    b @L_800F85C0
@L_800F85A8:
    lwz r8, 0x0(r5)
    cmplwi r8, 0x2
    beq @L_800F85C0
    li r8, 0x2
    li r0, 0x1
    stw r8, 0x0(r5)
@L_800F85C0:
    lwz r8, 0x64(r4)
    cmplwi r8, 0x0
    beq @L_800F85E4
    lwz r8, 0x64(r4)
    subic. r8, r8, 0x1
    stw r8, 0x64(r4)
    bne @L_800F85E4
    li r8, 0x2
    stw r8, 0x5c(r4)
@L_800F85E4:
    lbz r8, 0x68(r4)
    cmplwi r8, 0x0
    beq @L_800F8624
    lwz r9, 0x60(r4)
    lbz r8, 0x68(r4)
    cmplw r9, r8
    bge @L_800F8614
    li r9, 0x2
    li r8, 0x0
    stw r9, 0x5c(r4)
    stb r8, 0x68(r4)
    b @L_800F8624
@L_800F8614:
    lbz r9, 0x68(r4)
    lwz r8, 0x60(r4)
    subf r8, r9, r8
    stw r8, 0x60(r4)
@L_800F8624:
    addi r5, r5, 0x4
    addi r6, r6, 0x4
    addi r7, r7, 0x1
    bdnz @L_800F8454
    clrlwi. r0, r0, 24
    beq @L_800F8644
    addi r3, r3, 0x1c0
    bl fn_800AB4FC
@L_800F8644:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
