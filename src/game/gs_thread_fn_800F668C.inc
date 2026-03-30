    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x14(r3)
    addi r0, r3, 0x1
    stw r0, 0x14(r31)
    lwz r4, 0x28(r31)
    lwz r3, 0x14(r31)
    cmpwi r4, 0x40
    lwz r5, 0x0(r3)
    ble @L_800F66D4
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F66E8
    @L_800F66D4:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    stw r5, 0x6c(r3)
    @L_800F66E8:
    lwz r4, 0x14(r31)
    li r3, 0x1
    addi r0, r4, 0x4
    stw r0, 0x14(r31)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
