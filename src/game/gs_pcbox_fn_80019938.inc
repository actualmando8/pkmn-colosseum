    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80103FE4
    lha r0, 0x6(r31)
    li r4, 0x0
    cmpwi r0, 0xe95
    beq @L_80019998
    bge @L_8001997C
    cmpwi r0, 0xe93
    beq @L_80019988
    bge @L_80019990
    b @L_800199A4
    @L_8001997C:
    cmpwi r0, 0xe97
    bge @L_800199A4
    b @L_800199A0
    @L_80019988:
    li r4, 0x0
    b @L_800199A4
    @L_80019990:
    li r4, 0x1
    b @L_800199A4
    @L_80019998:
    li r4, 0x2
    b @L_800199A4
    @L_800199A0:
    li r4, 0x3
    @L_800199A4:
    mulli r0, r4, 0xc
    add r3, r3, r0
    lwz r4, 0x4(r3)
    cmplwi r4, 0x0
    beq @L_800199DC
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    @L_800199DC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
