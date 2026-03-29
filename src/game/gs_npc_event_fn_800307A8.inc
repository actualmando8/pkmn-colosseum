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
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_800307F4
    lhz r5, 0x0(r4)
    @L_800307F4:
    addi r4, r4, 0x14
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_8003080C
    lhz r5, 0x0(r4)
    @L_8003080C:
    cmpwi r5, 0x2
    beq @L_80030840
    bge @L_8003085C
    cmpwi r5, 0x1
    bge @L_80030824
    b @L_8003085C
    @L_80030824:
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r6, r3
    b @L_8003085C
    @L_80030840:
    lwz r0, lbl_8047A420(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r6, r3
    @L_8003085C:
    cmplwi r6, 0x0
    beq @L_800308B0
    mr r3, r6
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 24
    li r3, 0x2f
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x4414
    bl fn_800FBB34
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_800308BC
    @L_800308B0:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_800308BC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
