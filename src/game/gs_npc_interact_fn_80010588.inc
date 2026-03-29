    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    bl fn_80103FE4
    mr r31, r3
    mr r3, r28
    bl fn_801040A0
    lha r0, 0x6(r29)
    li r30, 0x0
    cmpwi r0, 0xc6
    beq @L_800105FC
    bge @L_800105E0
    cmpwi r0, 0xc4
    beq @L_800105EC
    bge @L_800105F4
    b @L_80010608
    @L_800105E0:
    cmpwi r0, 0xc8
    bge @L_80010608
    b @L_80010604
    @L_800105EC:
    li r30, 0x0
    b @L_80010608
    @L_800105F4:
    li r30, 0x1
    b @L_80010608
    @L_800105FC:
    li r30, 0x2
    b @L_80010608
    @L_80010604:
    li r30, 0x3
    @L_80010608:
    lbz r0, 0x2(r3)
    extsb r0, r0
    cmpw r0, r30
    bne @L_80010648
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    li r6, 0x49
    li r7, 0x0
    bl fn_801040F0
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    li r6, 0x4a
    li r7, 0x0
    bl fn_801040F0
    @L_80010648:
    mulli r0, r30, 0xc
    add r3, r31, r0
    lwz r4, 0x4(r3)
    cmplwi r4, 0x0
    beq @L_80010684
    li r3, 0x37
    bl fn_80132A38
    mr r3, r28
    mr r4, r29
    bl fn_8001D834
    mr r5, r3
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    @L_80010684:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
