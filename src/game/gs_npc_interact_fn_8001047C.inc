    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_80103FE4
    mr r31, r3
    lwz r3, 0x4(r29)
    bl fn_80104530
    srwi r0, r3, 16
    sth r0, 0x8(r1)
    lbz r0, 0x9(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_8001056C
    cmpwi r0, 0x4
    bge @L_8001056C
    mulli r30, r0, 0xc
    add r3, r31, r30
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_8001056C
    lwz r3, 0x40(r31)
    bl fn_80205B8C
    cmplwi r3, 0x0
    beq @L_80010508
    lbz r6, 0x95(r29)
    li r4, 0x0
    li r5, 0x7f
    extsb r6, r6
    bl fn_8012640C
    clrlwi r0, r3, 16
    b @L_8001050C
    @L_80010508:
    li r0, 0x0
    @L_8001050C:
    clrlwi r0, r0, 16
    cmpwi r0, 0x164
    beq @L_80010534
    bge @L_80010528
    cmpwi r0, 0x0
    beq @L_80010534
    b @L_80010544
    @L_80010528:
    cmpwi r0, 0x166
    bge @L_80010544
    b @L_8001053C
    @L_80010534:
    li r6, 0x0
    b @L_8001054C
    @L_8001053C:
    li r6, 0x5d
    b @L_8001054C
    @L_80010544:
    add r3, r31, r30
    lhz r6, 0xc(r3)
    @L_8001054C:
    clrlwi r0, r6, 16
    cmplwi r0, 0x0
    beq @L_8001056C
    mr r5, r29
    li r3, 0x0
    li r4, 0x2
    li r7, 0x0
    bl fn_801040F0
    @L_8001056C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
