    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    lis r4, lbl_80266700@ha
    li r0, 0xe
    addi r4, r4, lbl_80266700@l
    addi r6, r1, 0x4
    subi r5, r4, 0x4
    mtctr r0
    @L_8000CF8C:
    lwz r4, 0x4(r5)
    lwzu r0, 0x8(r5)
    stw r4, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8000CF8C
    addi r4, r1, 0x8
    li r5, 0x0
    li r0, 0x2
    mtctr r0
    @L_8000CFB0:
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x8(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x10(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x18(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x20(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x28(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r5, r5, 0x1
    lwz r0, 0x30(r4)
    cmpw r3, r0
    beq @L_8000D028
    addi r4, r4, 0x38
    addi r5, r5, 0x1
    bdnz @L_8000CFB0
    @L_8000D028:
    cmpwi r5, 0xe
    blt @L_8000D038
    li r3, 0x0
    b @L_8000D04C
    @L_8000D038:
    slwi r0, r5, 3
    addi r3, r1, 0xc
    lwzx r3, r3, r0
    bl fn_8002DC6C
    li r3, 0x0
    @L_8000D04C:
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
