    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r3
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lwz r3, 0x28(r3)
    cmpwi r3, 0x0
    bgt @L_800F6810
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x10(r1)
    b @L_800F6828
    @L_800F6810:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F6828:
    lwz r3, 0x28(r31)
    lwz r0, 0x10(r1)
    cmpwi r3, 0x0
    stw r0, 0x1c(r1)
    bgt @L_800F6858
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0xc(r1)
    b @L_800F6870
    @L_800F6858:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F6870:
    lwz r0, 0xc(r1)
    stw r0, 0x1c(r31)
    lwz r3, 0x28(r31)
    stw r0, 0x18(r1)
    cmpwi r3, 0x0
    bgt @L_800F68A4
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x8(r1)
    b @L_800F68BC
    @L_800F68A4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F68BC:
    lwz r0, 0x8(r1)
    lis r3, lbl_8027107C@ha
    lwz r30, 0x1c(r1)
    addi r29, r3, lbl_8027107C@l
    stw r0, 0x14(r1)
    li r28, 0x0
    stw r0, 0x14(r31)
    b @L_800F6904
    @L_800F68DC:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F68F8
    mr r3, r29
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6900
    @L_800F68F8:
    subi r0, r3, 0x1
    stw r0, 0x28(r31)
    @L_800F6900:
    addi r28, r28, 0x1
    @L_800F6904:
    cmpw r28, r30
    blt @L_800F68DC
    lwz r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_800F6928
    li r0, 0x3
    li r3, 0x0
    stb r0, 0x4(r31)
    b @L_800F692C
    @L_800F6928:
    li r3, 0x1
    @L_800F692C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
