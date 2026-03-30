    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    lis r4, lbl_802666E0@ha
    li r12, 0x0
    addi r11, r4, lbl_802666E0@l
    lwz r10, 0x0(r11)
    lwz r9, 0x4(r11)
    lwz r8, 0x8(r11)
    lwz r7, 0xc(r11)
    lwz r6, 0x10(r11)
    lwz r5, 0x14(r11)
    lwz r4, 0x18(r11)
    lwz r0, 0x1c(r11)
    stw r10, 0x8(r1)
    stw r9, 0xc(r1)
    stw r8, 0x10(r1)
    stw r7, 0x14(r1)
    stw r6, 0x18(r1)
    stw r5, 0x1c(r1)
    stw r4, 0x20(r1)
    stw r0, 0x24(r1)
    cmpw r3, r10
    beq @L_8000D0E4
    li r12, 0x1
    cmpw r3, r8
    beq @L_8000D0E4
    li r12, 0x2
    cmpw r3, r6
    beq @L_8000D0E4
    li r12, 0x3
    cmpw r3, r4
    beq @L_8000D0E4
    li r12, 0x4
    @L_8000D0E4:
    cmpwi r12, 0x4
    blt @L_8000D0F4
    li r3, 0x0
    b @L_8000D10C
    @L_8000D0F4:
    slwi r0, r12, 3
    addi r3, r1, 0xc
    lwzx r3, r3, r0
    li r4, 0x0
    bl fn_80029760
    li r3, 0x0
    @L_8000D10C:
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
