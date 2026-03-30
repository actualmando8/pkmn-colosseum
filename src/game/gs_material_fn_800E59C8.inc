    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lhz r0, 0x150(r3)
    cmplwi r0, 0x0
    bne @L_800E59F8
    li r0, 0x0
    b @L_800E5A28
    @L_800E59F8:
    lwz r3, 0x14c(r30)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_800E5A10
    li r0, 0x0
    b @L_800E5A28
    @L_800E5A10:
    bl fn_800DF240
    clrlwi. r0, r3, 31
    beq @L_800E5A24
    li r0, 0x1
    b @L_800E5A28
    @L_800E5A24:
    li r0, 0x0
    @L_800E5A28:
    clrlwi. r0, r0, 24
    beq @L_800E5A44
    lwz r3, 0x14c(r30)
    mr r4, r31
    lwz r3, 0x0(r3)
    bl fn_800DF11C
    b @L_800E5A5C
    @L_800E5A44:
    li r3, 0x7f
    li r0, 0xff
    stb r3, 0x2(r31)
    stb r3, 0x1(r31)
    stb r3, 0x0(r31)
    stb r0, 0x3(r31)
    @L_800E5A5C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
