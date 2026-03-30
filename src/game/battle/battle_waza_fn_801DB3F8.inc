    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r6, -32728
    stw r0, 0x14(r1)
    addi r7, r6, -26744
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lwz r8, 0xc(r4)
    lhz r0, 0x2e(r4)
    mulli r6, r8, 0xd4
    lwz r3, 0x2c(r3)
    cmpwi r0, 0x2
    lwz r31, 0x10(r4)
    add r30, r3, r6
    beq @L_801DB580
    bge @L_801DB444
    cmpwi r0, 0x1
    bge @L_801DB450
    b @L_801DB818
@L_801DB444:
    cmpwi r0, 0x5
    bge @L_801DB818
    b @L_801DB6A8
@L_801DB450:
    cmpwi r8, 0xb
    beq @L_801DB480
    bge @L_801DB474
    cmpwi r8, 0x0
    beq @L_801DB480
    blt @L_801DB50C
    cmpwi r8, 0x8
    bge @L_801DB4F8
    b @L_801DB50C
@L_801DB474:
    cmpwi r8, 0x10
    beq @L_801DB480
    b @L_801DB50C
@L_801DB480:
    rlwinm r0, r5, 0, 24, 31
    cmpwi r0, 0x5
    beq @L_801DB4C8
    bge @L_801DB4A8
    cmpwi r0, 0x0
    beq @L_801DB4C0
    blt @L_801DB4E4
    cmpwi r0, 0x3
    bge @L_801DB4D4
    b @L_801DB4DC
@L_801DB4A8:
    cmpwi r0, 0x7
    beq @L_801DB4B8
    bge @L_801DB4E4
    b @L_801DB4DC
@L_801DB4B8:
    li r3, 0x0
    b @L_801DB830
@L_801DB4C0:
    li r0, 0x0
    b @L_801DB82C
@L_801DB4C8:
    addi r3, r7, 0x0
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DB4D4:
    lwz r0, 0xc(r30)
    b @L_801DB82C
@L_801DB4DC:
    lwz r0, 0x10(r30)
    b @L_801DB82C
@L_801DB4E4:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB4F8:
    addi r3, r7, 0x88
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB50C:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x7
    bgt @L_801DB56C
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 10048
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    b @L_801DB830
    li r0, 0x0
    b @L_801DB82C
    addi r3, r7, 0x0
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r0, 0xc(r30)
    b @L_801DB82C
    lwz r0, 0x10(r30)
    b @L_801DB82C
    lwz r0, 0x14(r30)
    b @L_801DB82C
    lwz r0, 0x18(r30)
    b @L_801DB82C
@L_801DB56C:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB580:
    cmpwi r8, 0xb
    beq @L_801DB5B0
    bge @L_801DB5A4
    cmpwi r8, 0x0
    beq @L_801DB5B0
    blt @L_801DB694
    cmpwi r8, 0x8
    bge @L_801DB628
    b @L_801DB694
@L_801DB5A4:
    cmpwi r8, 0x10
    beq @L_801DB5B0
    b @L_801DB694
@L_801DB5B0:
    rlwinm r0, r5, 0, 24, 31
    cmpwi r0, 0x5
    beq @L_801DB604
    bge @L_801DB5D8
    cmpwi r0, 0x0
    beq @L_801DB5F0
    blt @L_801DB614
    cmpwi r0, 0x3
    bge @L_801DB5F8
    b @L_801DB60C
@L_801DB5D8:
    cmpwi r0, 0x7
    beq @L_801DB5E8
    bge @L_801DB614
    b @L_801DB60C
@L_801DB5E8:
    li r3, 0x0
    b @L_801DB830
@L_801DB5F0:
    li r0, 0x0
    b @L_801DB82C
@L_801DB5F8:
    addi r3, r7, 0xdc
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DB604:
    lwz r0, 0xc(r30)
    b @L_801DB82C
@L_801DB60C:
    lwz r0, 0x10(r30)
    b @L_801DB82C
@L_801DB614:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB628:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x7
    bgt @L_801DB680
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 10016
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    b @L_801DB830
    li r0, 0x0
    b @L_801DB82C
    addi r3, r7, 0xdc
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r0, 0xc(r30)
    b @L_801DB82C
    lwz r0, 0x10(r30)
    b @L_801DB82C
    lwz r0, 0x14(r30)
    b @L_801DB82C
@L_801DB680:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB694:
    addi r3, r7, 0x88
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB6A8:
    cmpwi r8, 0xb
    beq @L_801DB6D8
    bge @L_801DB6CC
    cmpwi r8, 0x0
    beq @L_801DB6D8
    blt @L_801DB7B0
    cmpwi r8, 0x8
    bge @L_801DB73C
    b @L_801DB7B0
@L_801DB6CC:
    cmpwi r8, 0x10
    beq @L_801DB6D8
    b @L_801DB7B0
@L_801DB6D8:
    rlwinm r0, r5, 0, 24, 31
    cmpwi r0, 0x6
    beq @L_801DB720
    bge @L_801DB700
    cmpwi r0, 0x0
    beq @L_801DB710
    blt @L_801DB728
    cmpwi r0, 0x3
    bge @L_801DB718
    b @L_801DB720
@L_801DB700:
    cmpwi r0, 0x8
    bge @L_801DB728
    li r3, 0x0
    b @L_801DB830
@L_801DB710:
    li r0, 0x0
    b @L_801DB82C
@L_801DB718:
    lwz r0, 0xc(r30)
    b @L_801DB82C
@L_801DB720:
    lwz r0, 0x10(r30)
    b @L_801DB82C
@L_801DB728:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB73C:
    rlwinm r0, r5, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_801DB794
    bge @L_801DB75C
    cmpwi r0, 0x0
    beq @L_801DB77C
    bge @L_801DB78C
    b @L_801DB79C
@L_801DB75C:
    cmpwi r0, 0x7
    beq @L_801DB774
    bge @L_801DB79C
    cmpwi r0, 0x6
    bge @L_801DB794
    b @L_801DB784
@L_801DB774:
    li r3, 0x0
    b @L_801DB830
@L_801DB77C:
    li r0, 0x0
    b @L_801DB82C
@L_801DB784:
    lwz r0, 0xc(r30)
    b @L_801DB82C
@L_801DB78C:
    lwz r0, 0x10(r30)
    b @L_801DB82C
@L_801DB794:
    lwz r0, 0x14(r30)
    b @L_801DB82C
@L_801DB79C:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB7B0:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x7
    bgt @L_801DB804
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 9984
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    b @L_801DB830
    li r0, 0x0
    b @L_801DB82C
    lwz r0, 0xc(r30)
    b @L_801DB82C
    lwz r0, 0x10(r30)
    b @L_801DB82C
    lwz r0, 0x14(r30)
    b @L_801DB82C
    lwz r0, 0x18(r30)
    b @L_801DB82C
@L_801DB804:
    addi r3, r7, 0x4c
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB818:
    addi r3, r7, 0x128
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801DB830
@L_801DB82C:
    add r3, r31, r0
@L_801DB830:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
