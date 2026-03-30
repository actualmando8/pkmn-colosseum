    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl OSDisableInterrupts
    li r6, 0x0
    b @L_801BF5B4
    @L_801BF590:
    mulli r5, r6, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x4
    bne @L_801BF5B0
    b @L_801BF5C0
    @L_801BF5B0:
    addi r6, r6, 0x1
    @L_801BF5B4:
    cmpwi r6, 0x3
    blt @L_801BF590
    li r6, -0x1
    @L_801BF5C0:
    mr r31, r6
    cmpwi r31, -0x1
    bne @L_801BF690
    li r6, 0x0
    b @L_801BF5F8
    @L_801BF5D4:
    mulli r5, r6, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x5
    bne @L_801BF5F4
    b @L_801BF604
    @L_801BF5F4:
    addi r6, r6, 0x1
    @L_801BF5F8:
    cmpwi r6, 0x3
    blt @L_801BF5D4
    li r6, -0x1
    @L_801BF604:
    mr r31, r6
    cmpwi r31, -0x1
    bne @L_801BF690
    li r6, 0x0
    b @L_801BF63C
    @L_801BF618:
    mulli r5, r6, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x6
    bne @L_801BF638
    b @L_801BF648
    @L_801BF638:
    addi r6, r6, 0x1
    @L_801BF63C:
    cmpwi r6, 0x3
    blt @L_801BF618
    li r6, -0x1
    @L_801BF648:
    mr r31, r6
    cmpwi r31, -0x1
    bne @L_801BF690
    li r6, 0x0
    b @L_801BF680
    @L_801BF65C:
    mulli r5, r6, 0x60
    lis r4, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    lwz r0, 0x5c(r4)
    cmpwi r0, 0x7
    bne @L_801BF67C
    b @L_801BF68C
    @L_801BF67C:
    addi r6, r6, 0x1
    @L_801BF680:
    cmpwi r6, 0x3
    blt @L_801BF65C
    li r6, -0x1
    @L_801BF68C:
    mr r31, r6
    @L_801BF690:
    bl OSRestoreInterrupts
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
