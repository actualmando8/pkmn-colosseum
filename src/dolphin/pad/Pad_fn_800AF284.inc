    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    mulli r4, r29, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r4
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800AF340
    li r31, 0x0
    stw r31, 0x0(r30)
    addi r3, r29, 0x0
    li r4, 0x0
    bl fn_8009870C
    addi r3, r30, 0xe0
    bl fn_8009A590
    lwz r0, 0xcc(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800AF2FC
    stw r31, 0xcc(r30)
    mtlr r12
    addi r3, r29, 0x0
    li r4, -0x3
    blrl
    @L_800AF2FC:
    lwz r0, 0x4(r30)
    cmpwi r0, -0x1
    beq @L_800AF310
    li r0, -0x3
    stw r0, 0x4(r30)
    @L_800AF310:
    lwz r12, 0xc4(r30)
    cmplwi r12, 0x0
    beq @L_800AF340
    lwz r0, 0x24(r30)
    cmpwi r0, 0x7
    blt @L_800AF340
    li r0, 0x0
    mtlr r12
    stw r0, 0xc4(r30)
    addi r3, r29, 0x0
    li r4, -0x3
    blrl
    @L_800AF340:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
