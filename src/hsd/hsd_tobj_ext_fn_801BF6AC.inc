    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_80466BC0@ha
    lis r4, lbl_802756F8@ha
    stw r0, 0x24(r1)
    addi r3, r3, lbl_80466BC0@l
    stw r31, 0x1c(r1)
    addi r31, r4, lbl_802756F8@l
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r0, 0x1d4(r3)
    cmpwi r0, 0x2
    blt @L_801BF880
    bl OSDisableInterrupts
    li r5, 0x0
    mr r29, r3
    b @L_801BF718
@L_801BF6F4:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x3
    bne @L_801BF714
    b @L_801BF724
@L_801BF714:
    addi r5, r5, 0x1
@L_801BF718:
    cmpwi r5, 0x3
    blt @L_801BF6F4
    li r5, -0x1
@L_801BF724:
    mr r28, r5
    cmpwi r28, -0x1
    bne @L_801BF740
    addi r5, r31, 0x0
    addi r3, r2, -22384
    li r4, 0x30d
    bl fn_80196E10
@L_801BF740:
    bl OSDisableInterrupts
    mulli r5, r28, 0x60
    lis r4, lbl_80466BC0@ha
    mr r30, r3
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x3
    beq @L_801BF774
    addi r5, r31, 0xc
    addi r3, r2, -22384
    li r4, 0x260
    bl fn_80196E10
@L_801BF774:
    mulli r6, r28, 0x60
    lis r3, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    addi r5, r3, lbl_80466BC0@l
    lis r3, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    mulli r4, r28, 0x60
    add r5, r5, r6
    addi r3, r3, lbl_80466BC0@l
    li r6, 0x4
    add r4, r0, r4
    li r0, 0xb
    stw r6, 0x5c(r5)
    addi r5, r4, 0x5c
    subi r4, r3, 0x4
    mtctr r0
@L_801BF7B4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801BF7B4
    lis r3, lbl_80466BC0@ha
    li r0, 0x0
    addi r4, r3, lbl_80466BC0@l
    mr r3, r30
    stb r0, 0x54(r4)
    bl OSRestoreInterrupts
    bl OSDisableInterrupts
    mulli r5, r28, 0x60
    lis r4, lbl_80466BC0@ha
    mr r30, r3
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x4
    beq @L_801BF814
    addi r5, r31, 0x38
    addi r3, r2, -22384
    li r4, 0x2e4
    bl fn_80196E10
@L_801BF814:
    li r5, 0x0
    b @L_801BF840
@L_801BF81C:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x6
    bne @L_801BF83C
    b @L_801BF84C
@L_801BF83C:
    addi r5, r5, 0x1
@L_801BF840:
    cmpwi r5, 0x3
    blt @L_801BF81C
    li r5, -0x1
@L_801BF84C:
    cmpwi r5, -0x1
    li r6, 0x6
    beq @L_801BF85C
    li r6, 0x5
@L_801BF85C:
    mulli r5, r28, 0x60
    lis r4, lbl_80466BC0@ha
    mr r3, r30
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    stw r6, 0x5c(r4)
    bl OSRestoreInterrupts
    mr r3, r29
    bl OSRestoreInterrupts
@L_801BF880:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

