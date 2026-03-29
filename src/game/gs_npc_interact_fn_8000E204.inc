    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    lha r3, 0x6(r31)
    bl fn_8005D8B8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000E268
    lwz r3, 0x4(r30)
    bl fn_801022B8
    lha r0, 0x6(r31)
    cmpw r0, r3
    bne @L_8000E258
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8000E274
    @L_8000E258:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8000E274
    @L_8000E268:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_8000E274:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
