    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    bne cr1, @L_8019C3FC
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
@L_8019C3FC:
    lwz r0, lbl_8047B280(r13)
    lis r11, lbl_80274818@ha
    stw r3, 0x8(r1)
    addi r31, r11, lbl_80274818@l
    cmpwi r0, 0x0
    li r30, 0x0
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    beq @L_8019C45C
    lwz r0, lbl_8047B28C(r13)
    cmpwi r0, 0x0
    bne @L_8019C454
    addi r3, r31, 0x8c
    crxor 6, 6, 6
    bl OSReport
    li r0, 0x1
    stw r0, lbl_8047B28C(r13)
@L_8019C454:
    mr r3, r30
    b @L_8019C678
@L_8019C45C:
    addi r4, r1, 0x98
    addi r0, r1, 0x8
    lis r5, 0x100
    cmplwi r3, 0x7
    stw r4, 0x6c(r1)
    stw r5, 0x68(r1)
    stw r0, 0x70(r1)
    bgt @L_8019C674
    lis r4, jumptable_8036C8C0@ha
    rlwinm r0, r3, 2, 0, 29
    addi r3, r4, jumptable_8036C8C0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8019C674
    stw r0, lbl_80478C7C(r13)
    li r30, 0x1
    b @L_8019C674
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8019C674
    stw r0, lbl_80478C80(r13)
    li r30, 0x1
    b @L_8019C674
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r31, 0x0(r3)
    bl fn_8009F3D4
    cmplw r31, r3
    bge @L_8019C674
    stw r31, lbl_8047B284(r13)
    li r30, 0x1
    b @L_8019C674
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r31, 0x0(r4)
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    li r30, 0x1
    stw r31, lbl_8047B270(r13)
    stw r0, lbl_8047B274(r13)
    b @L_8019C674
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r0, 0x0(r4)
    li r4, 0x1
    stw r0, 0x74(r1)
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r0, 0x0(r4)
    li r4, 0x1
    stw r0, 0x78(r1)
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r0, 0x0(r4)
    li r4, 0x1
    stw r0, 0x7c(r1)
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r0, 0x0(r4)
    li r4, 0x1
    stw r0, 0x80(r1)
    bl __va_arg
    mr r4, r3
    addi r3, r1, 0x68
    lwz r0, 0x0(r4)
    li r4, 0x1
    stw r0, 0x84(r1)
    bl __va_arg
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8019C5E4
    addi r3, r31, 0xc8
    crxor 6, 6, 6
    bl OSReport
    addi r3, r31, 0xec
    crxor 6, 6, 6
    bl OSReport
    b @L_8019C674
@L_8019C5E4:
    addi r3, r1, 0x74
    li r4, 0x14
    bl fn_801A69C0
    li r0, 0x1
    li r30, 0x1
    stw r0, lbl_8047B288(r13)
    b @L_8019C674
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8019C674
    stw r0, lbl_80478C74(r13)
    li r30, 0x1
    b @L_8019C674
    addi r3, r31, 0xc8
    crxor 6, 6, 6
    bl OSReport
    addi r3, r31, 0x128
    crxor 6, 6, 6
    bl OSReport
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_8019C674
    li r30, 0x1
    b @L_8019C674
    addi r3, r31, 0xc8
    crxor 6, 6, 6
    bl OSReport
    addi r3, r31, 0x160
    crxor 6, 6, 6
    bl OSReport
@L_8019C674:
    mr r3, r30
@L_8019C678:
    lwz r0, 0x94(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr

