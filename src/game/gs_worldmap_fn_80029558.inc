    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    cmpwi r29, 0x2
    li r31, 0x1
    beq @L_800295A0
    bge @L_80029594
    cmpwi r29, 0x0
    bge @L_800295E4
    b @L_800295E0
    @L_80029594:
    cmpwi r29, 0x4
    bge @L_800295E0
    b @L_800295C8
    @L_800295A0:
    clrlwi r5, r30, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800295E4
    li r31, 0x0
    b @L_800295E4
    @L_800295C8:
    cmpwi r30, 0x0
    blt @L_800295D8
    cmpwi r30, 0x3
    blt @L_800295E4
    @L_800295D8:
    li r31, 0x0
    b @L_800295E4
    @L_800295E0:
    li r31, 0x0
    @L_800295E4:
    cmpwi r31, 0x0
    bne @L_800295F4
    li r3, 0x0
    b @L_8002961C
    @L_800295F4:
    lis r3, lbl_803A2068@ha
    li r0, 0x0
    addi r31, r3, lbl_803A2068@l
    sth r0, 0x0(r31)
    stw r29, 0x18(r31)
    stw r30, 0x1c(r31)
    stw r0, 0x20(r31)
    stw r0, 0x28(r31)
    bl fn_80028FBC
    lwz r3, 0x20(r31)
    @L_8002961C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
