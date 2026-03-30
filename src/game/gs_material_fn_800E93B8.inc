    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r7, lbl_80401490@ha
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    addi r31, r7, lbl_80401490@l
    mr r27, r3
    mr r28, r4
    mr r29, r6
    li r3, 0x0
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800E93F4
    mr r30, r31
    b @L_800E944C
    @L_800E93F4:
    addi r30, r31, 0x58
    lwz r0, 0x58(r31)
    cmplwi r0, 0x0
    bne @L_800E9408
    b @L_800E944C
    @L_800E9408:
    lwzu r0, 0x58(r30)
    cmplwi r0, 0x0
    bne @L_800E9418
    b @L_800E944C
    @L_800E9418:
    lwzu r0, 0x58(r30)
    cmplwi r0, 0x0
    bne @L_800E9428
    b @L_800E944C
    @L_800E9428:
    lwzu r0, 0x58(r30)
    cmplwi r0, 0x0
    bne @L_800E9438
    b @L_800E944C
    @L_800E9438:
    lwzu r0, 0x58(r30)
    cmplwi r0, 0x0
    bne @L_800E9448
    b @L_800E944C
    @L_800E9448:
    mr r30, r3
    @L_800E944C:
    cmplwi r30, 0x0
    bne @L_800E98B0
    mr r3, r27
    mr r4, r28
    li r6, 0x0
    bl fn_800E9998
    mr. r30, r3
    beq @L_800E9504
    li r0, 0x4
    li r4, 0x0
    mtctr r0
    @L_800E9478:
    lwz r5, 0x8(r3)
    cmplwi r5, 0x0
    beq @L_800E9498
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E9498
    li r30, 0x0
    b @L_800E9504
    @L_800E9498:
    lwz r5, 0xc(r3)
    cmplwi r5, 0x0
    beq @L_800E94B8
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E94B8
    li r30, 0x0
    b @L_800E9504
    @L_800E94B8:
    lwz r5, 0x10(r3)
    cmplwi r5, 0x0
    beq @L_800E94D8
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E94D8
    li r30, 0x0
    b @L_800E9504
    @L_800E94D8:
    lwz r5, 0x14(r3)
    cmplwi r5, 0x0
    beq @L_800E94F8
    lwz r0, 0x0(r5)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E94F8
    li r30, 0x0
    b @L_800E9504
    @L_800E94F8:
    addi r3, r3, 0x10
    addi r4, r4, 0x3
    bdnz @L_800E9478
    @L_800E9504:
    cmplwi r30, 0x0
    bne @L_800E9668
    li r5, 0x0
    li r7, 0x0
    mr r4, r5
    @L_800E9518:
    lwz r0, 0x0(r31)
    mr r30, r31
    cmplw r0, r27
    bne @L_800E964C
    li r0, 0x2
    addi r3, r4, 0x8
    li r7, 0x1
    li r8, 0x0
    mtctr r0
    @L_800E953C:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E955C
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E955C
    li r7, 0x0
    b @L_800E9644
    @L_800E955C:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E957C
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E957C
    li r7, 0x0
    b @L_800E9644
    @L_800E957C:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E959C
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E959C
    li r7, 0x0
    b @L_800E9644
    @L_800E959C:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E95BC
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E95BC
    li r7, 0x0
    b @L_800E9644
    @L_800E95BC:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E95DC
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E95DC
    li r7, 0x0
    b @L_800E9644
    @L_800E95DC:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E95FC
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E95FC
    li r7, 0x0
    b @L_800E9644
    @L_800E95FC:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E961C
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E961C
    li r7, 0x0
    b @L_800E9644
    @L_800E961C:
    lwzx r6, r31, r3
    cmplwi r6, 0x0
    beq @L_800E963C
    lwz r0, 0x0(r6)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_800E963C
    li r7, 0x0
    b @L_800E9644
    @L_800E963C:
    addi r8, r8, 0x7
    bdnz @L_800E953C
    @L_800E9644:
    clrlwi. r0, r7, 24
    bne @L_800E9660
    @L_800E964C:
    addi r5, r5, 0x1
    addi r4, r4, 0x4
    cmplwi r5, 0x6
    addi r31, r31, 0x58
    blt @L_800E9518
    @L_800E9660:
    clrlwi. r0, r7, 24
    beq @L_800E9984
    @L_800E9668:
    li r0, 0x2
    mr r4, r30
    li r3, 0x0
    mtctr r0
    @L_800E9678:
    lwz r0, 0x8(r4)
    cmplw r0, r28
    bne @L_800E9688
    b @L_800E9740
    @L_800E9688:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E96A0
    b @L_800E9740
    @L_800E96A0:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E96B8
    b @L_800E9740
    @L_800E96B8:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E96D0
    b @L_800E9740
    @L_800E96D0:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E96E8
    b @L_800E9740
    @L_800E96E8:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E9700
    b @L_800E9740
    @L_800E9700:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E9718
    b @L_800E9740
    @L_800E9718:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplw r0, r28
    bne @L_800E9730
    b @L_800E9740
    @L_800E9730:
    addi r4, r4, 0x4
    addi r3, r3, 0x1
    bdnz @L_800E9678
    li r3, -0x1
    @L_800E9740:
    cmpwi r3, -0x1
    bne @L_800E9984
    li r0, 0x2
    mr r4, r30
    li r3, 0x0
    mtctr r0
    @L_800E9758:
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800E9768
    b @L_800E9820
    @L_800E9768:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E9780
    b @L_800E9820
    @L_800E9780:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E9798
    b @L_800E9820
    @L_800E9798:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E97B0
    b @L_800E9820
    @L_800E97B0:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E97C8
    b @L_800E9820
    @L_800E97C8:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E97E0
    b @L_800E9820
    @L_800E97E0:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E97F8
    b @L_800E9820
    @L_800E97F8:
    lwz r0, 0xc(r4)
    addi r3, r3, 0x1
    addi r4, r4, 0x4
    cmplwi r0, 0x0
    bne @L_800E9810
    b @L_800E9820
    @L_800E9810:
    addi r4, r4, 0x4
    addi r3, r3, 0x1
    bdnz @L_800E9758
    li r3, -0x1
    @L_800E9820:
    slwi r0, r3, 2
    mr r3, r29
    add r5, r30, r0
    addi r4, r1, 0x8
    stw r28, 0x8(r5)
    bl fn_80190E34
    lfs f1, 0x8(r1)
    lfs f0, 0xc(r1)
    fcmpo cr0, f0, f1
    ble @L_800E984C
    fmr f1, f0
    @L_800E984C:
    lfs f0, 0x10(r1)
    fcmpo cr0, f0, f1
    ble @L_800E985C
    fmr f1, f0
    @L_800E985C:
    lwz r3, 0xc(r29)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    lwz r0, 0x48(r30)
    cmplw r3, r0
    bge @L_800E989C
    stw r3, 0x48(r30)
    @L_800E989C:
    lwz r0, 0x4c(r30)
    cmplw r3, r0
    ble @L_800E9984
    stw r3, 0x4c(r30)
    b @L_800E9984
    @L_800E98B0:
    stw r27, 0x0(r30)
    mr r3, r29
    addi r4, r1, 0x20
    stw r5, 0x4(r30)
    bl fn_80190E34
    lfs f1, 0x20(r1)
    lfs f0, 0x24(r1)
    fcmpo cr0, f0, f1
    ble @L_800E98D8
    fmr f1, f0
    @L_800E98D8:
    lfs f0, 0x28(r1)
    fcmpo cr0, f0, f1
    ble @L_800E98E8
    fmr f1, f0
    @L_800E98E8:
    lwz r3, 0xc(r29)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    stw r3, 0x48(r30)
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_80190E34
    lfs f1, 0x14(r1)
    lfs f0, 0x18(r1)
    fcmpo cr0, f0, f1
    ble @L_800E993C
    fmr f1, f0
    @L_800E993C:
    lfs f0, 0x1c(r1)
    fcmpo cr0, f0, f1
    ble @L_800E994C
    fmr f1, f0
    @L_800E994C:
    lwz r3, 0xc(r29)
    lfs f0, lbl_8047CBC0(r2)
    lfs f3, 0x0(r3)
    lfs f2, 0x4(r3)
    lfs f4, 0x8(r3)
    fadds f2, f3, f2
    fadds f2, f4, f2
    fdivs f0, f2, f0
    fmuls f1, f1, f0
    bl fn_800CDA74
    frsp f1, f1
    bl fn_800C46B0
    stw r3, 0x4c(r30)
    stw r28, 0x8(r30)
    @L_800E9984:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
