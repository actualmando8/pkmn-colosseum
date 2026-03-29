    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80010BF0
    li r4, 0x18
    bl fn_80103FFC
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80010B78
    lwz r4, 0x60(r30)
    li r5, 0x18
    bl memcpy
    @L_80010B78:
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    beq @L_80010BBC
    mr r3, r30
    li r4, 0xb6
    bl fn_801046C8
    li r0, 0x13d
    li r4, 0xb8
    stw r0, 0x4c(r3)
    mr r3, r30
    bl fn_801046C8
    li r0, 0x140
    li r4, 0x1
    stw r0, 0x4c(r3)
    li r3, 0xb8
    bl fn_8005D8F8
    b @L_80010BF0
    @L_80010BBC:
    mr r3, r30
    li r4, 0xb6
    bl fn_801046C8
    li r0, 0x13f
    li r4, 0xb8
    stw r0, 0x4c(r3)
    mr r3, r30
    bl fn_801046C8
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    li r3, 0xb8
    bl fn_8005D8F8
    @L_80010BF0:
    mr r3, r30
    bl fn_80103FE4
    mr r31, r3
    lwz r3, 0x4(r30)
    bl fn_801022B8
    cmpwi r3, 0xb7
    beq @L_80010C5C
    bge @L_80010C20
    cmpwi r3, 0xb5
    beq @L_80010C2C
    bge @L_80010C38
    b @L_80010C74
    @L_80010C20:
    cmpwi r3, 0xb9
    bge @L_80010C74
    b @L_80010C68
    @L_80010C2C:
    li r0, 0x0
    stw r0, 0x80(r30)
    b @L_80010C7C
    @L_80010C38:
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    beq @L_80010C50
    li r0, 0x1
    stw r0, 0x80(r30)
    b @L_80010C7C
    @L_80010C50:
    li r0, 0x3
    stw r0, 0x80(r30)
    b @L_80010C7C
    @L_80010C5C:
    li r0, 0x2
    stw r0, 0x80(r30)
    b @L_80010C7C
    @L_80010C68:
    li r0, 0x3
    stw r0, 0x80(r30)
    b @L_80010C7C
    @L_80010C74:
    li r0, -0x1
    stw r0, 0x80(r30)
    @L_80010C7C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
