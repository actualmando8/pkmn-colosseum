    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_8017A624
    cmpwi r3, 0x0
    beq @L_8017C548
    bl OSDisableInterrupts
    lis r5, lbl_80453FEC@ha
    mr r30, r3
    addi r5, r5, lbl_80453FEC@l
    lwz r31, 0x1c(r5)
    lwz r0, 0x48(r31)
    cmpwi r0, 0xc8
    beq @L_8017C4F0
    bge @L_8017C49C
    cmpwi r0, 0x4
    beq @L_8017C524
    bge @L_8017C484
    cmpwi r0, 0x2
    beq @L_8017C524
    bge @L_8017C4D8
    cmpwi r0, 0x1
    bge @L_8017C4CC
    b @L_8017C514
    @L_8017C484:
    cmpwi r0, 0x65
    beq @L_8017C4E4
    bge @L_8017C514
    cmpwi r0, 0x64
    bge @L_8017C524
    b @L_8017C514
    @L_8017C49C:
    cmpwi r0, 0x12f
    beq @L_8017C4FC
    bge @L_8017C4C0
    cmpwi r0, 0x12d
    beq @L_8017C524
    bge @L_8017C514
    cmpwi r0, 0xca
    bge @L_8017C514
    b @L_8017C524
    @L_8017C4C0:
    cmpwi r0, 0x190
    beq @L_8017C508
    b @L_8017C514
    @L_8017C4CC:
    li r0, 0x2
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C4D8:
    li r0, 0x4
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C4E4:
    li r0, 0x96
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C4F0:
    li r0, 0xc9
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C4FC:
    li r0, 0x130
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C508:
    li r0, 0x191
    stw r0, 0x48(r31)
    b @L_8017C524
    @L_8017C514:
    li r3, 0x3e8
    li r0, 0x1
    stw r3, 0x48(r31)
    stw r0, 0x44(r31)
    @L_8017C524:
    lwz r0, 0x6c(r31)
    cmplwi r0, 0x0
    beq @L_8017C540
    lwz r3, 0x6c(r31)
    bl fn_80167E64
    li r0, 0x0
    stw r0, 0x6c(r31)
    @L_8017C540:
    mr r3, r30
    bl OSRestoreInterrupts
    @L_8017C548:
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
