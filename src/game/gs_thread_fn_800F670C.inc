    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    lwz r3, 0x28(r3)
    cmpwi r3, 0x0
    bgt @L_800F6748
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x8(r1)
    b @L_800F6760
    @L_800F6748:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F6760:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    stw r0, 0xc(r1)
    beq @L_800F6780
    lwz r3, 0x14(r31)
    addi r0, r3, 0x4
    stw r0, 0x14(r31)
    b @L_800F6794
    @L_800F6780:
    lwz r3, 0x14(r31)
    lwz r4, 0x0(r31)
    lwz r0, 0x0(r3)
    add r0, r4, r0
    stw r0, 0x14(r31)
    @L_800F6794:
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
