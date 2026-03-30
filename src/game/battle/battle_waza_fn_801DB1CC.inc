    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lhz r4, 0x72(r3)
    cmplwi r4, 0x0
    beq @L_801DB26C
    lis r3, -32698
    li r5, 0x0
    addi r3, r3, 31936
    lhz r0, 0x4(r3)
    lwz r3, 0x0(r3)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DB22C
@L_801DB214:
    lhz r0, 0x72(r3)
    cmplw r0, r4
    bne @L_801DB224
    addi r5, r5, 0x1
@L_801DB224:
    addi r3, r3, 0x8c
    bdnz @L_801DB214
@L_801DB22C:
    cmpwi r5, 0x1
    bne @L_801DB264
    mr r30, r4
    mr r31, r4
    b @L_801DB244
@L_801DB240:
    bl fn_800F0308
@L_801DB244:
    mr r3, r31
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_801DB240
    mr r3, r30
    bl fn_800F915C
    mr r3, r30
    bl fn_8017B1CC
@L_801DB264:
    li r0, 0x0
    sth r0, 0x72(r29)
@L_801DB26C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
