    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, 0x60(r3)
    lis r6, lbl_802672D8@ha
    addi r10, r6, lbl_802672D8@l
    lwz r0, 0x0(r5)
    lwz r9, 0x0(r10)
    cmpwi r0, 0x0
    lwz r8, 0x4(r10)
    lwz r7, 0x8(r10)
    lwz r6, 0xc(r10)
    lwz r5, 0x10(r10)
    beq @L_8004DAA4
    li r0, 0x0
    b @L_8004DB14
    @L_8004DAA4:
    li r10, 0x0
    lha r0, 0x6(r4)
    cmpw r0, r9
    beq @L_8004DAE8
    li r10, 0x1
    cmpw r0, r8
    beq @L_8004DAE8
    li r10, 0x2
    cmpw r0, r7
    beq @L_8004DAE8
    li r10, 0x3
    cmpw r0, r6
    beq @L_8004DAE8
    li r10, 0x4
    cmpw r0, r5
    beq @L_8004DAE8
    li r10, 0x5
    @L_8004DAE8:
    cmpwi r10, 0x5
    blt @L_8004DAF8
    li r3, 0x0
    b @L_8004DB24
    @L_8004DAF8:
    lbz r0, 0x95(r3)
    extsb r0, r0
    cmpw r0, r10
    bne @L_8004DB10
    li r0, 0x1
    b @L_8004DB14
    @L_8004DB10:
    li r0, 0x0
    @L_8004DB14:
    mr r3, r4
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x0
    @L_8004DB24:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
