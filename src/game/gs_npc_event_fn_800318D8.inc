    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_80266E90@ha
    li r7, 0x0
    addi r5, r3, lbl_80266E90@l
    li r8, 0x0
    li r6, 0x0
    li r0, 0x2
    mtctr r0
    @L_80031900:
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_80031918
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_80031918:
    addi r5, r5, 0x12
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_80031934
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_80031934:
    addi r5, r5, 0x12
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_80031950
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_80031950:
    addi r5, r5, 0x12
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_8003196C
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_8003196C:
    addi r5, r5, 0x12
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_80031988
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_80031988:
    addi r5, r5, 0x12
    lha r3, 0x6(r4)
    lhz r0, 0xe(r5)
    cmpw r3, r0
    bne @L_800319A4
    lbz r7, 0x0(r5)
    lbz r8, 0x1(r5)
    @L_800319A4:
    addi r5, r5, 0x12
    addi r6, r6, 0x5
    bdnz @L_80031900
    cmpwi r7, 0x2
    beq @L_800319E4
    bge @L_80031A00
    cmpwi r7, 0x1
    bge @L_800319C8
    b @L_80031A00
    @L_800319C8:
    lwz r0, lbl_8047A424(r13)
    cmpw r0, r8
    bne @L_80031A00
    mr r3, r4
    li r4, 0x1
    bl fn_80109220
    b @L_80031A0C
    @L_800319E4:
    lwz r0, lbl_8047A420(r13)
    cmpw r0, r8
    bne @L_80031A00
    mr r3, r4
    li r4, 0x1
    bl fn_80109220
    b @L_80031A0C
    @L_80031A00:
    mr r3, r4
    li r4, 0x0
    bl fn_80109220
    @L_80031A0C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
