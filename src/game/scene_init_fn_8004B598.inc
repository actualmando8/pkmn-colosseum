    nofralloc
    stwu r1, -0x20(r1)
    lis r3, lbl_80267190@ha
    cmpwi r5, 0x75f
    addi r7, r3, lbl_80267190@l
    lwz r6, 0x0(r7)
    lwz r3, 0x4(r7)
    lwz r0, 0x8(r7)
    stw r6, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    beq @L_8004B648
    bge @L_8004B5F4
    cmpwi r5, 0x6e9
    beq @L_8004B648
    bge @L_8004B5E4
    cmpwi r5, 0x6e7
    beq @L_8004B620
    bge @L_8004B634
    b @L_8004B658
    @L_8004B5E4:
    cmpwi r5, 0x75d
    beq @L_8004B620
    bge @L_8004B634
    b @L_8004B658
    @L_8004B5F4:
    cmpwi r5, 0x763
    beq @L_8004B620
    bge @L_8004B610
    cmpwi r5, 0x761
    beq @L_8004B634
    bge @L_8004B648
    b @L_8004B620
    @L_8004B610:
    cmpwi r5, 0x765
    beq @L_8004B648
    bge @L_8004B658
    b @L_8004B634
    @L_8004B620:
    lis r3, lbl_803A6A60@ha
    li r0, 0x0
    addi r3, r3, lbl_803A6A60@l
    stw r0, 0x44(r3)
    b @L_8004B658
    @L_8004B634:
    lis r3, lbl_803A6A60@ha
    li r0, 0x1
    addi r3, r3, lbl_803A6A60@l
    stw r0, 0x44(r3)
    b @L_8004B658
    @L_8004B648:
    lis r3, lbl_803A6A60@ha
    li r0, 0x2
    addi r3, r3, lbl_803A6A60@l
    stw r0, 0x44(r3)
    @L_8004B658:
    lis r3, lbl_803A6A60@ha
    addi r3, r3, lbl_803A6A60@l
    lbz r5, 0x0(r3)
    extsb r0, r5
    cmpwi r0, 0x0
    bge @L_8004B678
    lbz r0, 0x48(r3)
    b @L_8004B680
    @L_8004B678:
    mr r0, r5
    stb r5, 0x48(r3)
    @L_8004B680:
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8004B6F4
    bge @L_8004B6A0
    cmpwi r0, 0x0
    beq @L_8004B6AC
    bge @L_8004B6D0
    b @L_8004B73C
    @L_8004B6A0:
    cmpwi r0, 0x4
    bge @L_8004B73C
    b @L_8004B718
    @L_8004B6AC:
    lis r5, lbl_803A6A60@ha
    lis r3, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A6A60@l
    lwz r5, 0x44(r5)
    addi r0, r3, lbl_802EF0A8@l
    addi r3, r5, 0x6e7
    mulli r3, r3, 0x1c
    add r7, r0, r3
    b @L_8004B74C
    @L_8004B6D0:
    lis r5, lbl_803A6A60@ha
    lis r3, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A6A60@l
    lwz r5, 0x44(r5)
    addi r0, r3, lbl_802EF0A8@l
    addi r3, r5, 0x75d
    mulli r3, r3, 0x1c
    add r7, r0, r3
    b @L_8004B74C
    @L_8004B6F4:
    lis r5, lbl_803A6A60@ha
    lis r3, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A6A60@l
    lwz r5, 0x44(r5)
    addi r0, r3, lbl_802EF0A8@l
    addi r3, r5, 0x760
    mulli r3, r3, 0x1c
    add r7, r0, r3
    b @L_8004B74C
    @L_8004B718:
    lis r5, lbl_803A6A60@ha
    lis r3, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A6A60@l
    lwz r5, 0x44(r5)
    addi r0, r3, lbl_802EF0A8@l
    addi r3, r5, 0x763
    mulli r3, r3, 0x1c
    add r7, r0, r3
    b @L_8004B74C
    @L_8004B73C:
    lis r3, lbl_802EF0A8@ha
    addi r3, r3, lbl_802EF0A8@l
    addis r7, r3, 0x1
    subi r7, r7, 0x3ebc
    @L_8004B74C:
    lis r5, lbl_803A6A60@ha
    addi r3, r1, 0x8
    addi r5, r5, lbl_803A6A60@l
    lfs f1, lbl_8047BDF0(r2)
    lwz r0, 0x44(r5)
    addi r6, r5, 0xc
    lfs f2, 0x8(r5)
    slwi r0, r0, 2
    lfsx f0, r3, r0
    lfsx f3, r6, r0
    fdivs f0, f1, f0
    fmuls f0, f2, f0
    fadds f0, f3, f0
    stfsx f0, r6, r0
    lwz r0, 0x44(r5)
    slwi r0, r0, 2
    lfsx f0, r6, r0
    fcmpo cr0, f0, f1
    ble @L_8004B7A0
    fsubs f0, f0, f1
    stfsx f0, r6, r0
    @L_8004B7A0:
    lwz r0, 0x44(r5)
    lfs f0, lbl_8047BDAC(r2)
    slwi r0, r0, 2
    lfsx f1, r6, r0
    fcmpo cr0, f1, f0
    bge @L_8004B7C4
    lfs f0, lbl_8047BDF0(r2)
    fadds f0, f1, f0
    stfsx f0, r6, r0
    @L_8004B7C4:
    lwz r0, 0x44(r5)
    slwi r0, r0, 2
    lfsx f0, r6, r0
    stfs f0, 0x70(r4)
    lha r0, 0x2(r7)
    sth r0, 0x50(r4)
    lha r0, 0x4(r7)
    sth r0, 0x52(r4)
    addi r1, r1, 0x20
    blr
