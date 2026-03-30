    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r29, 0x0
    stw r28, 0x10(r1)
    mr r28, r3
    lis r3, lbl_8027107C@ha
    lwz r4, 0x14(r28)
    addi r31, r3, lbl_8027107C@l
    addi r0, r4, 0x1
    stw r0, 0x14(r28)
    lwz r3, 0x14(r28)
    lhz r30, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r28)
    b @L_800F6B28
    @L_800F6B00:
    lwz r3, 0x28(r28)
    cmpwi r3, 0x0
    bgt @L_800F6B1C
    mr r3, r31
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6B24
    @L_800F6B1C:
    subi r0, r3, 0x1
    stw r0, 0x28(r28)
    @L_800F6B24:
    addi r29, r29, 0x1
    @L_800F6B28:
    cmpw r29, r30
    blt @L_800F6B00
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
