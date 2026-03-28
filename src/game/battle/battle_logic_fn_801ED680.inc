    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    stw r28, -21084(r13)
    lwz r3, 0x0(r3)
    subis r0, r3, 0x7b1e
    cmplwi r0, 0xe3f0
    bne @L_801ED718
    addi r4, r28, 0x1c
    lis r3, -32697
    stw r4, -21080(r13)
    addi r31, r3, -10704
    li r30, 0x0
    lhz r0, 0x6(r28)
    mulli r0, r0, 0xc
    add r29, r4, r0
    b @L_801ED708
@L_801ED6D8:
    lwz r0, 0x4(r29)
    add r0, r0, r28
    stw r0, 0x4(r29)
    lwz r3, 0x4(r29)
    bl fn_800EFD3C
    cmplwi r3, 0x0
    beq @L_801ED718
    lhz r0, 0x0(r29)
    addi r29, r29, 0x8
    addi r30, r30, 0x1
    slwi r0, r0, 2
    stwx r3, r31, r0
@L_801ED708:
    lwz r3, -21084(r13)
    lhz r0, 0x4(r3)
    cmpw r30, r0
    blt @L_801ED6D8
@L_801ED718:
    li r0, 0x1
    stb r0, -21088(r13)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
