    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x158(r3)
    cmplw r30, r0
    beq @L_800E9098
    lhz r3, 0x164(r29)
    cmplwi r3, 0x0
    beq @L_800E9064
    bl fn_800E24B0
    lhz r3, 0x164(r29)
    bl fn_800E209C
    li r0, 0x0
    sth r0, 0x164(r29)
    stw r0, 0x158(r29)
    stw r0, 0x15c(r29)
    @L_800E9064:
    cmpwi r30, 0x0
    beq @L_800E90AC
    cmplwi r31, 0x0
    bne @L_800E9078
    b @L_800E90AC
    @L_800E9078:
    stw r30, 0x158(r29)
    lwz r0, 0x158(r29)
    slwi r3, r0, 2
    bl fn_800E3534
    sth r3, 0x164(r29)
    lhz r3, 0x164(r29)
    bl fn_800E27B0
    stw r3, 0x15c(r29)
    @L_800E9098:
    lwz r0, 0x158(r29)
    mr r4, r31
    lwz r3, 0x15c(r29)
    slwi r5, r0, 2
    bl memcpy
    @L_800E90AC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
