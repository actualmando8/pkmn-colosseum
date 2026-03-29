    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    bl fn_80104704
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80011C5C
    bl fn_80103FE4
    mr r31, r3
    mr r3, r30
    bl fn_801040A0
    lwz r0, 0x20(r31)
    cmplw r29, r0
    ble @L_80011C14
    subf r0, r0, r29
    b @L_80011C18
    @L_80011C14:
    subf r0, r29, r0
    @L_80011C18:
    mulli r4, r0, 0x64
    lwz r0, 0x1c(r31)
    divwu r0, r4, r0
    extsh r0, r0
    sth r0, 0xc(r3)
    lha r0, 0xc(r3)
    cmpwi r0, 0xf
    bge @L_80011C40
    li r0, 0xf
    sth r0, 0xc(r3)
    @L_80011C40:
    lwz r4, 0x20(r31)
    li r0, 0x0
    stw r4, 0x8(r3)
    stw r29, 0x20(r31)
    sth r0, 0xe(r3)
    li r3, 0x4d0
    bl fn_80166A28
    @L_80011C5C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
