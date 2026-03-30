    nofralloc
    lwz r8, 0x2c(r3)
    addis r0, r8, 0x1
    cmplwi r0, 0xffff
    bne @L_800B535C
    li r0, 0x0
    stb r0, 0x2e(r4)
    li r8, 0x0
    sth r0, 0x34(r4)
    sth r0, 0x36(r4)
    @L_800B535C:
    lbz r0, 0x7(r3)
    li r9, 0x0
    clrlwi r0, r0, 30
    cmpwi r0, 0x2
    beq @L_800B5394
    bge @L_800B53A8
    cmpwi r0, 0x1
    bge @L_800B5380
    b @L_800B53A8
    @L_800B5380:
    stw r8, 0x3c(r4)
    addi r0, r8, 0xc00
    addi r8, r8, 0xe00
    stw r0, 0x40(r4)
    b @L_800B53B4
    @L_800B5394:
    stw r8, 0x3c(r4)
    li r0, -0x1
    addi r8, r8, 0x1800
    stw r0, 0x40(r4)
    b @L_800B53B4
    @L_800B53A8:
    li r0, -0x1
    stw r0, 0x3c(r4)
    stw r0, 0x40(r4)
    @L_800B53B4:
    li r0, 0x2
    mtctr r0
    addi r7, r4, 0x0
    li r10, 0x0
    li r6, 0x0
    li r0, -0x1
    @L_800B53CC:
    lhz r5, 0x30(r3)
    sraw r5, r5, r6
    clrlwi r5, r5, 30
    cmpwi r5, 0x2
    beq @L_800B5400
    bge @L_800B540C
    cmpwi r5, 0x1
    bge @L_800B53F0
    b @L_800B540C
    @L_800B53F0:
    stw r8, 0x44(r7)
    li r9, 0x1
    addi r8, r8, 0x400
    b @L_800B5410
    @L_800B5400:
    stw r8, 0x44(r7)
    addi r8, r8, 0x800
    b @L_800B5410
    @L_800B540C:
    stw r0, 0x44(r7)
    @L_800B5410:
    lhz r5, 0x30(r3)
    addi r6, r6, 0x2
    addi r7, r7, 0x4
    sraw r5, r5, r6
    clrlwi r5, r5, 30
    cmpwi r5, 0x2
    beq @L_800B544C
    bge @L_800B5458
    cmpwi r5, 0x1
    bge @L_800B543C
    b @L_800B5458
    @L_800B543C:
    stw r8, 0x44(r7)
    li r9, 0x1
    addi r8, r8, 0x400
    b @L_800B545C
    @L_800B544C:
    stw r8, 0x44(r7)
    addi r8, r8, 0x800
    b @L_800B545C
    @L_800B5458:
    stw r0, 0x44(r7)
    @L_800B545C:
    lhz r5, 0x30(r3)
    addi r6, r6, 0x2
    addi r10, r10, 0x1
    sraw r5, r5, r6
    clrlwi r5, r5, 30
    cmpwi r5, 0x2
    addi r7, r7, 0x4
    beq @L_800B549C
    bge @L_800B54A8
    cmpwi r5, 0x1
    bge @L_800B548C
    b @L_800B54A8
    @L_800B548C:
    stw r8, 0x44(r7)
    li r9, 0x1
    addi r8, r8, 0x400
    b @L_800B54AC
    @L_800B549C:
    stw r8, 0x44(r7)
    addi r8, r8, 0x800
    b @L_800B54AC
    @L_800B54A8:
    stw r0, 0x44(r7)
    @L_800B54AC:
    lhz r5, 0x30(r3)
    addi r6, r6, 0x2
    addi r10, r10, 0x1
    sraw r5, r5, r6
    clrlwi r5, r5, 30
    cmpwi r5, 0x2
    addi r7, r7, 0x4
    beq @L_800B54EC
    bge @L_800B54F8
    cmpwi r5, 0x1
    bge @L_800B54DC
    b @L_800B54F8
    @L_800B54DC:
    stw r8, 0x44(r7)
    li r9, 0x1
    addi r8, r8, 0x400
    b @L_800B54FC
    @L_800B54EC:
    stw r8, 0x44(r7)
    addi r8, r8, 0x800
    b @L_800B54FC
    @L_800B54F8:
    stw r0, 0x44(r7)
    @L_800B54FC:
    addi r6, r6, 0x2
    addi r7, r7, 0x4
    addi r10, r10, 0x1
    bdnz @L_800B53CC
    cmpwi r9, 0x0
    beq @L_800B5520
    stw r8, 0x64(r4)
    addi r8, r8, 0x200
    b @L_800B5528
    @L_800B5520:
    li r0, -0x1
    stw r0, 0x64(r4)
    @L_800B5528:
    stw r8, 0x68(r4)
    blr
