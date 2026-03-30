    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, 0x28(r3)
    cmpwi r5, 0x40
    ble @L_800F6B80
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6B98
    @L_800F6B80:
    addi r4, r5, 0x1
    slwi r0, r5, 2
    stw r4, 0x28(r3)
    add r3, r3, r0
    li r0, -0x1
    stw r0, 0x6c(r3)
    @L_800F6B98:
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
