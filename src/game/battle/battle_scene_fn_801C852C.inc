    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_801C860C
    cmpwi r29, 0x1
    beq @L_801C857C
    bge @L_801C860C
    cmpwi r29, 0x0
    bge @L_801C8570
    b @L_801C860C
@L_801C8570:
    li r3, 0x0
    bl fn_801D0AFC
    b @L_801C860C
@L_801C857C:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r29, 0x0
    mr r31, r3
    b @L_801C8600
@L_801C8594:
    mr r3, r31
    mr r4, r29
    bl fn_8012AC08
    mr r30, r3
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C85FC
    mr r3, r30
    li r4, 0x3
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C85D8
    mr r3, r30
    li r4, 0x3
    bl fn_80121B4C
@L_801C85D8:
    mr r3, r30
    li r4, 0x4
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C85FC
    mr r3, r30
    li r4, 0x4
    bl fn_80121B4C
@L_801C85FC:
    addi r29, r29, 0x1
@L_801C8600:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801C8594
@L_801C860C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
