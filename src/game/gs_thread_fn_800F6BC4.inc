    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_80271068@ha
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    addi r30, r4, lbl_80271068@l
    @L_800F6BE4:
    lbz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800F6C04
    lwz r4, 0x8(r31)
    addi r3, r30, 0x120
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800F6C8C
    @L_800F6C04:
    cmplwi r0, 0x3
    bne @L_800F6C18
    li r0, 0x0
    stb r0, 0x4(r31)
    b @L_800F6C8C
    @L_800F6C18:
    lwz r3, 0x14(r31)
    addi r0, r3, 0x1
    stw r0, 0x14(r31)
    lbz r4, 0x0(r3)
    cmpwi r4, 0x26
    blt @L_800F6C40
    addi r3, r30, 0x150
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6C64
    @L_800F6C40:
    lis r3, lbl_803155D0@ha
    clrlslwi r0, r4, 24, 2
    addi r3, r3, lbl_803155D0@l
    lwzx r12, r3, r0
    cmplwi r12, 0x0
    beq @L_800F6C64
    mr r3, r31
    mtctr r12
    bctrl
    @L_800F6C64:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    ble @L_800F6BE4
    subic. r3, r3, 0x1
    addi r0, r3, 0x1
    mtctr r0
    blt @L_800F6BE4
    @L_800F6C80:
    subi r3, r3, 0x1
    bdnz @L_800F6C80
    b @L_800F6BE4
    @L_800F6C8C:
    lbz r0, 0x4(r31)
    cmplwi r0, 0x4
    bne @L_800F6CA0
    li r0, 0x0
    stb r0, 0x4(r31)
    @L_800F6CA0:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F6CC4
    addi r3, r30, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x8(r1)
    b @L_800F6CDC
    @L_800F6CC4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F6CDC:
    lwz r12, 0x10(r31)
    lwz r4, 0x8(r1)
    cmplwi r12, 0x0
    stw r4, 0xc(r1)
    beq @L_800F6CFC
    mr r3, r31
    mtctr r12
    bctrl
    @L_800F6CFC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r3, 0xc(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
