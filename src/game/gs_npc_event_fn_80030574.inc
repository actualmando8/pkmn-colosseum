    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r4
    lwz r5, 0x64(r29)
    lis r4, lbl_80266F68@ha
    lbz r0, 0x8b(r3)
    addi r3, r4, lbl_80266F68@l
    clrrwi r4, r5, 8
    li r27, 0x0
    or r31, r4, r0
    li r5, 0x0
    lha r4, 0x6(r29)
    lhz r0, 0x8(r3)
    cmpw r4, r0
    bne @L_800305C0
    lhz r5, 0x0(r3)
    li r30, 0x0
    @L_800305C0:
    lhz r0, 0xa(r3)
    cmpw r4, r0
    bne @L_800305D4
    lhz r5, 0x0(r3)
    li r30, 0x1
    @L_800305D4:
    lhz r0, 0xc(r3)
    cmpw r4, r0
    bne @L_800305E8
    lhz r5, 0x0(r3)
    li r30, 0x2
    @L_800305E8:
    lhz r0, 0xe(r3)
    cmpw r4, r0
    bne @L_800305FC
    lhz r5, 0x0(r3)
    li r30, 0x3
    @L_800305FC:
    addi r3, r3, 0x14
    lha r4, 0x6(r29)
    lhz r0, 0x8(r3)
    cmpw r4, r0
    bne @L_80030618
    lhz r5, 0x0(r3)
    li r30, 0x0
    @L_80030618:
    lhz r0, 0xa(r3)
    cmpw r4, r0
    bne @L_8003062C
    lhz r5, 0x0(r3)
    li r30, 0x1
    @L_8003062C:
    lhz r0, 0xc(r3)
    cmpw r4, r0
    bne @L_80030640
    lhz r5, 0x0(r3)
    li r30, 0x2
    @L_80030640:
    lhz r0, 0xe(r3)
    cmpw r4, r0
    bne @L_80030654
    lhz r5, 0x0(r3)
    li r30, 0x3
    @L_80030654:
    cmpwi r5, 0x2
    beq @L_80030688
    bge @L_800306A4
    cmpwi r5, 0x1
    bge @L_8003066C
    b @L_800306A4
    @L_8003066C:
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r27, r3
    b @L_800306A4
    @L_80030688:
    lwz r0, lbl_8047A420(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r27, r3
    @L_800306A4:
    cmplwi r27, 0x0
    beq @L_80030788
    mr r3, r27
    clrlwi r6, r30, 16
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r27
    clrlwi r4, r30, 16
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800306E0
    li r28, 0x0
    @L_800306E0:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80030714
    bge @L_80030704
    cmpwi r4, 0x0
    beq @L_80030730
    b @L_8003071C
    @L_80030704:
    cmpw r4, r3
    bge @L_8003071C
    li r4, 0x933
    b @L_80030730
    @L_80030714:
    li r4, 0x934
    b @L_80030730
    @L_8003071C:
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r4, r3
    @L_80030730:
    cmplwi r4, 0x0
    beq @L_80030778
    mr r3, r4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x54(r29)
    mr r7, r31
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xe7
    bl fn_800FBB34
    mr r3, r29
    li r4, 0x1
    bl fn_80109220
    b @L_80030794
    @L_80030778:
    mr r3, r29
    li r4, 0x0
    bl fn_80109220
    b @L_80030794
    @L_80030788:
    mr r3, r29
    li r4, 0x0
    bl fn_80109220
    @L_80030794:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
