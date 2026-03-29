    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    clrlwi r31, r4, 24
    @L_80011B68:
    mr r3, r30
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80011B80
    li r3, 0x0
    b @L_80011BAC
    @L_80011B80:
    bl fn_801040A0
    lha r0, 0xc(r3)
    cmpwi r0, 0x0
    bne @L_80011B98
    li r3, 0x0
    b @L_80011BAC
    @L_80011B98:
    cmplwi r31, 0x0
    beq @L_80011BA8
    bl fn_800F0308
    b @L_80011B68
    @L_80011BA8:
    li r3, 0x1
    @L_80011BAC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
