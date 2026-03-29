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
    lha r0, 0x6(r29)
    li r30, 0x0
    cmpwi r0, 0x11cd
    beq @L_8001071C
    bge @L_800106F4
    cmpwi r0, 0xc1
    beq @L_80010738
    bge @L_800107FC
    cmpwi r0, 0xb4
    beq @L_80010700
    b @L_800107FC
    @L_800106F4:
    cmpwi r0, 0x11d8
    beq @L_80010780
    b @L_800107FC
    @L_80010700:
    bl fn_80103FE4
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    li r30, 0xcf
    b @L_800107FC
    @L_8001071C:
    bl fn_80103FE4
    mr r0, r3
    li r3, 0x36
    mr r4, r0
    bl fn_80132A38
    li r30, 0x196
    b @L_800107FC
    @L_80010738:
    bl fn_80103FE4
    mr r31, r3
    mr r3, r28
    bl fn_801040A0
    lbz r0, 0x0(r3)
    cmpwi r0, 0x1
    beq @L_80010778
    bge @L_800107FC
    cmpwi r0, 0x0
    bge @L_80010764
    b @L_800107FC
    @L_80010764:
    lwz r4, 0x0(r31)
    li r3, 0x36
    bl fn_80132A38
    li r30, 0x196
    b @L_800107FC
    @L_80010778:
    li r30, 0x199
    b @L_800107FC
    @L_80010780:
    lwz r0, 0x4(r28)
    li r30, 0x196
    cmpwi r0, 0xf7
    bne @L_800107A8
    bl fn_80103FE4
    mr r4, r3
    li r3, 0x36
    lwz r4, 0x0(r4)
    bl fn_80132A38
    b @L_800107FC
    @L_800107A8:
    cmpwi r0, 0xf8
    bne @L_80010824
    li r4, 0x1
    bl fn_801040D0
    mr r0, r3
    mr r3, r28
    mr r31, r0
    li r4, 0x2
    bl fn_801040D0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800107F0
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0x36
    bl fn_80132A38
    b @L_800107FC
    @L_800107F0:
    li r30, 0x1a9
    b @L_800107FC
    b @L_80010824
    @L_800107FC:
    cmplwi r30, 0x0
    beq @L_80010824
    mr r3, r28
    mr r4, r29
    bl fn_8001D834
    mr r5, r3
    mr r6, r30
    li r3, 0x0
    li r4, -0x2
    bl fn_800FB680
    @L_80010824:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
