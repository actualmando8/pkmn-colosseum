    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_801A432C
    mr r31, r3
    mr r30, r4
    b @L_801A4324
    @L_801A4294:
    cmplwi r31, 0x0
    beq @L_801A42FC
    cmplwi r30, 0x0
    beq @L_801A42FC
    lwz r0, 0x48(r31)
    cmplwi r0, 0x0
    beq @L_801A42B8
    lwz r3, 0x48(r31)
    bl fn_801C25E4
    @L_801A42B8:
    lwz r3, 0x4(r30)
    bl fn_801C2670
    cmplwi r31, 0x0
    stw r3, 0x48(r31)
    beq @L_801A42D4
    lwz r3, 0x18(r31)
    b @L_801A42D8
    @L_801A42D4:
    li r3, 0x0
    @L_801A42D8:
    lwz r4, 0x8(r30)
    bl fn_80191DCC
    cmplwi r31, 0x0
    beq @L_801A42F0
    lwz r3, 0x1c(r31)
    b @L_801A42F4
    @L_801A42F0:
    li r3, 0x0
    @L_801A42F4:
    lwz r4, 0xc(r30)
    bl fn_80191DCC
    @L_801A42FC:
    cmplwi r31, 0x0
    beq @L_801A430C
    lwz r31, 0xc(r31)
    b @L_801A4310
    @L_801A430C:
    li r31, 0x0
    @L_801A4310:
    cmplwi r30, 0x0
    beq @L_801A4320
    lwz r30, 0x0(r30)
    b @L_801A4324
    @L_801A4320:
    li r30, 0x0
    @L_801A4324:
    cmplwi r31, 0x0
    bne @L_801A4294
    @L_801A432C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
