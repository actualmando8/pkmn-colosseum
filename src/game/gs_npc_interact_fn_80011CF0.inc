    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    bl fn_80104704
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80011D80
    bl fn_80103FE4
    mr r31, r3
    mr r3, r30
    bl fn_801040A0
    lha r0, 0x1a(r31)
    extsh r4, r29
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80011D44
    neg r0, r0
    @L_80011D44:
    mulli r4, r0, 0x64
    lha r0, 0x18(r31)
    divw r0, r4, r0
    extsh r0, r0
    sth r0, 0x2(r3)
    lha r0, 0x2(r3)
    cmpwi r0, 0x0
    bgt @L_80011D6C
    li r0, 0x1
    sth r0, 0x2(r3)
    @L_80011D6C:
    lha r4, 0x1a(r31)
    li r0, 0x0
    sth r4, 0x0(r3)
    sth r29, 0x1a(r31)
    sth r0, 0x4(r3)
    @L_80011D80:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
