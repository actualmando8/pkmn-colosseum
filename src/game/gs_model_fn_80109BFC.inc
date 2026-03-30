    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80109C20
    li r3, 0x0
    b @L_80109C74
    @L_80109C20:
    li r0, 0x0
    stb r0, 0x1(r31)
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_80109C50
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_80109C68
    bl fn_801DB100
    li r0, 0x0
    stw r0, 0x24(r31)
    b @L_80109C68
    @L_80109C50:
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_80109C68
    bl fn_800E4BF4
    li r0, 0x0
    stw r0, 0x24(r31)
    @L_80109C68:
    li r0, 0x0
    li r3, 0x1
    stb r0, 0x0(r31)
    @L_80109C74:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
