    nofralloc
    stwu r1, -0x210(r1)
    lis r6, lbl_802730E0@ha
    addi r6, r6, lbl_802730E0@l
    li r5, 0x40
    stw r31, 0x20c(r1)
    addi r10, r1, 0x4
    subi r7, r6, 0x4
    mtctr r5
    @L_80142F18:
    lwz r6, 0x4(r7)
    lwzu r5, 0x8(r7)
    stw r6, 0x4(r10)
    stwu r5, 0x8(r10)
    bdnz @L_80142F18
    addi r7, r4, 0x6
    addi r5, r1, 0x8
    li r6, 0x0
    @L_80142F38:
    lwz r11, 0xc(r5)
    lwz r10, 0x4(r5)
    cmpwi r11, 0x3
    lbzx r12, r4, r10
    beq @L_80142F54
    cmpwi r11, 0x2
    bne @L_80142F88
    @L_80142F54:
    lbz r10, 0x8(r5)
    and. r10, r12, r10
    beq @L_80142F78
    cmpwi r11, 0x3
    lbz r0, 0x0(r7)
    addi r7, r7, 0x1
    bne @L_80142FD8
    extsb r8, r0
    b @L_80142FD8
    @L_80142F78:
    cmpwi r11, 0x3
    bne @L_80142FD8
    li r8, 0x0
    b @L_80142FD8
    @L_80142F88:
    lbz r10, 0x8(r5)
    li r31, 0x0
    and r12, r12, r10
    b @L_80142F9C
    @L_80142F98:
    addi r31, r31, 0x1
    @L_80142F9C:
    cmpwi r31, 0x8
    bge @L_80142FB0
    sraw r0, r10, r31
    clrlwi. r0, r0, 31
    beq @L_80142F98
    @L_80142FB0:
    clrlwi r0, r12, 24
    cmpwi r11, 0x0
    sraw r0, r0, r31
    clrlwi r0, r0, 24
    bne @L_80142FD8
    cmplwi r0, 0x0
    bne @L_80142FD4
    li r9, 0x0
    b @L_80142FD8
    @L_80142FD4:
    li r9, 0x1
    @L_80142FD8:
    lwz r10, 0x0(r5)
    cmplwi r10, 0x1f
    bgt @L_80143190
    lis r11, jumptable_80367E70@ha
    slwi r10, r10, 2
    addi r11, r11, jumptable_80367E70@l
    lwzx r10, r11, r10
    mtctr r10
    bctr
    lbz r10, 0x0(r3)
    rlwimi r10, r9, 7, 24, 24
    stb r10, 0x0(r3)
    b @L_80143190
    lbz r10, 0x0(r3)
    rlwimi r10, r9, 5, 26, 26
    stb r10, 0x0(r3)
    b @L_80143190
    lbz r10, 0x0(r3)
    rlwimi r10, r0, 1, 27, 30
    stb r10, 0x0(r3)
    b @L_80143190
    lbz r10, 0x1(r3)
    rlwimi r10, r0, 4, 24, 27
    stb r10, 0x1(r3)
    b @L_80143190
    lbz r10, 0x1(r3)
    rlwimi r10, r0, 0, 28, 31
    stb r10, 0x1(r3)
    b @L_80143190
    lbz r10, 0x2(r3)
    rlwimi r10, r0, 4, 24, 27
    stb r10, 0x2(r3)
    b @L_80143190
    lbz r10, 0x2(r3)
    rlwimi r10, r0, 0, 28, 31
    stb r10, 0x2(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 7, 24, 24
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 6, 25, 25
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 5, 26, 26
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 4, 27, 27
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 3, 28, 28
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 2, 29, 29
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 1, 30, 30
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x3(r3)
    rlwimi r10, r9, 0, 31, 31
    stb r10, 0x3(r3)
    b @L_80143190
    lbz r10, 0x4(r3)
    rlwimi r10, r9, 7, 24, 24
    stb r10, 0x4(r3)
    b @L_80143190
    stb r0, 0x8(r3)
    b @L_80143190
    stb r0, 0x9(r3)
    b @L_80143190
    lbz r10, 0x4(r3)
    rlwimi r10, r9, 6, 25, 25
    stb r10, 0x4(r3)
    b @L_80143190
    stb r0, 0xa(r3)
    b @L_80143190
    lbz r10, 0x4(r3)
    rlwimi r10, r9, 5, 26, 26
    stb r10, 0x4(r3)
    b @L_80143190
    stb r0, 0xb(r3)
    b @L_80143190
    lbz r10, 0x4(r3)
    rlwimi r10, r9, 4, 27, 27
    stb r10, 0x4(r3)
    b @L_80143190
    stb r0, 0xc(r3)
    b @L_80143190
    stb r0, 0xd(r3)
    b @L_80143190
    stb r0, 0xe(r3)
    b @L_80143190
    stb r0, 0xf(r3)
    b @L_80143190
    lbz r10, 0x4(r3)
    rlwimi r10, r9, 3, 28, 28
    stb r10, 0x4(r3)
    b @L_80143190
    stb r8, 0x5(r3)
    b @L_80143190
    stb r8, 0x6(r3)
    b @L_80143190
    stb r8, 0x7(r3)
    @L_80143190:
    addi r6, r6, 0x1
    addi r5, r5, 0x10
    cmpwi r6, 0x20
    blt @L_80142F38
    lwz r31, 0x20c(r1)
    addi r1, r1, 0x210
    blr
