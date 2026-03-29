    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r4
    lwz r4, 0x64(r31)
    li r6, 0x0
    lbz r0, 0x8b(r3)
    lis r3, lbl_80266F68@ha
    clrrwi r4, r4, 8
    mr r5, r6
    or r30, r4, r0
    addi r4, r3, lbl_80266F68@l
    lha r3, 0x6(r31)
    lhz r0, 0x12(r4)
    cmpw r3, r0
    bne @L_80030478
    lhz r5, 0x0(r4)
    @L_80030478:
    addi r4, r4, 0x14
    lha r3, 0x6(r31)
    lhz r0, 0x12(r4)
    cmpw r3, r0
    bne @L_80030490
    lhz r5, 0x0(r4)
    @L_80030490:
    cmpwi r5, 0x2
    beq @L_800304C4
    bge @L_800304E0
    cmpwi r5, 0x1
    bge @L_800304A8
    b @L_800304E0
    @L_800304A8:
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r6, r3
    b @L_800304E0
    @L_800304C4:
    lwz r0, lbl_8047A420(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r6, r3
    @L_800304E0:
    cmplwi r6, 0x0
    beq @L_80030550
    mr r3, r6
    bl fn_801230E0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80030540
    bl fn_801440A0
    bl fn_80144088
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r5, r30
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    li r0, 0x0
    mr r3, r31
    stw r0, 0x4c(r31)
    li r4, 0x1
    bl fn_80109220
    b @L_8003055C
    @L_80030540:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8003055C
    @L_80030550:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_8003055C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
