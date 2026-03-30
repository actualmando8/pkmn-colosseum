    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r4, 0x3
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r3, 0xc(r3)
    bl fn_801A68F8
    cmpwi r31, 0x2
    beq @L_800DCD60
    bge @L_800DCD34
    cmpwi r31, 0x0
    beq @L_800DCD40
    bge @L_800DCD50
    b @L_800DCD7C
    @L_800DCD34:
    cmpwi r31, 0x4
    bge @L_800DCD7C
    b @L_800DCD70
    @L_800DCD40:
    lwz r3, 0xc(r30)
    li r4, 0x0
    bl fn_801A6910
    b @L_800DCD7C
    @L_800DCD50:
    lwz r3, 0xc(r30)
    li r4, 0x1
    bl fn_801A6910
    b @L_800DCD7C
    @L_800DCD60:
    lwz r3, 0xc(r30)
    li r4, 0x2
    bl fn_801A6910
    b @L_800DCD7C
    @L_800DCD70:
    lwz r3, 0xc(r30)
    li r4, 0x3
    bl fn_801A6910
    @L_800DCD7C:
    stw r31, 0x4(r30)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
