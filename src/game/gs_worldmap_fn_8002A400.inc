    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r31, 0x60(r3)
    li r3, 0x50
    lwz r4, 0xc(r31)
    lwz r5, 0x8(r31)
    lwz r0, 0x0(r4)
    mullw r4, r5, r0
    bl fn_80132A38
    lwz r0, 0x14(r31)
    cmpwi r0, 0x0
    beq @L_8002A448
    li r31, 0x153
    b @L_8002A44C
    @L_8002A448:
    li r31, 0x151
    @L_8002A44C:
    mr r3, r31
    bl fn_800FA444
    lha r0, 0x54(r30)
    srwi r3, r3, 16
    mr r6, r31
    li r4, 0x0
    subf r3, r3, r0
    li r5, -0x1
    bl fn_800FB680
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
