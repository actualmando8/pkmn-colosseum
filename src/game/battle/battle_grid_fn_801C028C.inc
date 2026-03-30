    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r27, 0x7c(r1)
    mr r28, r3
    mr r27, r4
    mr r29, r5
    mr r31, r6
    mr r30, r7
    bne cr1, @L_801C02D4
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_801C02D4:
    cmplwi r28, 0x0
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    beq @L_801C0F0C
    addi r3, r1, 0x98
    addi r0, r1, 0x8
    lis r4, 0x500
    cmplwi r30, 0xb
    stw r3, 0x70(r1)
    stw r4, 0x6c(r1)
    stw r0, 0x74(r1)
    bgt @L_801C0380
    lis r3, jumptable_8036D974@ha
    slwi r0, r30, 2
    addi r3, r3, jumptable_8036D974@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    addi r3, r1, 0x6c
    li r4, 0x3
    bl __va_arg
    lfd f0, 0x0(r3)
    frsp f0, f0
    stfs f0, 0x68(r1)
    b @L_801C0394
    addi r3, r1, 0x6c
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    stw r0, 0x68(r1)
    b @L_801C0394
    addi r3, r1, 0x6c
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    stw r0, 0x68(r1)
    b @L_801C0394
    @L_801C0380:
    lis r4, lbl_80275780@ha
    la r3, lbl_8047DF40(r2)
    addi r5, r4, lbl_80275780@l
    li r4, 0x33a
    bl fn_80196D78
    @L_801C0394:
    cmplwi r27, 0xc
    bgt @L_801C0EF8
    lis r3, jumptable_8036D940@ha
    slwi r0, r27, 2
    addi r3, r3, jumptable_8036D940@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    addi r7, r1, 0x68
    bl fn_801C0F20
    b @L_801C0F0C
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    addi r7, r1, 0x68
    bl fn_801C1274
    b @L_801C0F0C
    cmplwi r28, 0x0
    beq @L_801C0F0C
    rlwinm. r0, r29, 0, 24, 24
    beq @L_801C053C
    lwz r0, 0x14(r28)
    cmplwi r0, 0x0
    beq @L_801C053C
    cmplwi r30, 0xb
    lwz r3, 0x14(r28)
    bgt @L_801C053C
    lis r4, jumptable_8036D910@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D910@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C053C
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0x8
    mtctr r12
    bctrl
    @L_801C053C:
    lwz r27, 0x8(r28)
    b @L_801C0690
    @L_801C0544:
    rlwinm. r0, r29, 0, 21, 21
    beq @L_801C068C
    lwz r0, 0x64(r27)
    cmplwi r0, 0x0
    beq @L_801C068C
    cmplwi r30, 0xb
    lwz r3, 0x64(r27)
    bgt @L_801C068C
    lis r4, jumptable_8036D8E0@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D8E0@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C068C
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    @L_801C068C:
    lwz r27, 0x8(r27)
    @L_801C0690:
    cmplwi r27, 0x0
    bne @L_801C0544
    b @L_801C0F0C
    mr r27, r28
    b @L_801C07F0
    @L_801C06A4:
    rlwinm. r0, r29, 0, 23, 23
    beq @L_801C07EC
    lwz r0, 0x18(r27)
    cmplwi r0, 0x0
    beq @L_801C07EC
    cmplwi r30, 0xb
    lwz r3, 0x18(r27)
    bgt @L_801C07EC
    lis r4, jumptable_8036D8B0@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D8B0@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C07EC
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0x9
    mtctr r12
    bctrl
    @L_801C07EC:
    lwz r27, 0x4(r27)
    @L_801C07F0:
    cmplwi r27, 0x0
    bne @L_801C06A4
    b @L_801C0F0C
    mr r27, r28
    b @L_801C0950
    @L_801C0804:
    rlwinm. r0, r29, 0, 21, 21
    beq @L_801C094C
    lwz r0, 0x64(r27)
    cmplwi r0, 0x0
    beq @L_801C094C
    cmplwi r30, 0xb
    lwz r3, 0x64(r27)
    bgt @L_801C094C
    lis r4, jumptable_8036D880@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D880@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C094C
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xb
    mtctr r12
    bctrl
    @L_801C094C:
    lwz r27, 0x8(r27)
    @L_801C0950:
    cmplwi r27, 0x0
    bne @L_801C0804
    b @L_801C0F0C
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    addi r7, r1, 0x68
    bl fn_801C1810
    b @L_801C0F0C
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    addi r7, r1, 0x68
    bl fn_801C1F00
    b @L_801C0F0C
    mr r27, r28
    b @L_801C0AE8
    @L_801C099C:
    rlwinm. r0, r29, 0, 22, 22
    beq @L_801C0AE4
    lwz r0, 0x18(r27)
    cmplwi r0, 0x0
    beq @L_801C0AE4
    cmplwi r30, 0xb
    lwz r3, 0x18(r27)
    bgt @L_801C0AE4
    lis r4, jumptable_8036D850@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D850@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0AE4
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C0AE4:
    lwz r27, 0x0(r27)
    @L_801C0AE8:
    cmplwi r27, 0x0
    bne @L_801C099C
    b @L_801C0F0C
    cmplwi r28, 0x0
    beq @L_801C0F0C
    rlwinm. r0, r29, 0, 20, 20
    beq @L_801C0C44
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801C0C44
    cmplwi r30, 0xb
    lwz r3, 0x18(r28)
    bgt @L_801C0C44
    lis r4, jumptable_8036D820@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D820@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C0C44
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0xc
    mtctr r12
    bctrl
    @L_801C0C44:
    lwz r27, 0x1c(r28)
    b @L_801C0D98
    @L_801C0C4C:
    rlwinm. r0, r29, 0, 22, 22
    beq @L_801C0D94
    lwz r0, 0x18(r27)
    cmplwi r0, 0x0
    beq @L_801C0D94
    cmplwi r30, 0xb
    lwz r3, 0x18(r27)
    bgt @L_801C0D94
    lis r4, jumptable_8036D7F0@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D7F0@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lfs f1, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C0D94
    mr r12, r31
    mr r4, r27
    lwz r6, 0x68(r1)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C0D94:
    lwz r27, 0x0(r27)
    @L_801C0D98:
    cmplwi r27, 0x0
    bne @L_801C0C4C
    b @L_801C0F0C
    rlwinm. r0, r29, 0, 27, 27
    beq @L_801C0F0C
    cmplwi r28, 0x0
    beq @L_801C0F0C
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_801C0F0C
    cmplwi r30, 0xb
    lwz r3, 0x1c(r28)
    bgt @L_801C0F0C
    lis r4, jumptable_8036D7C0@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D7C0@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r31
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    lwz r4, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    li r5, 0x5
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lwz r5, 0x68(r1)
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lfs f1, 0x68(r1)
    li r5, 0x5
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0x5
    mtctr r12
    bctrl
    b @L_801C0F0C
    mr r12, r31
    mr r4, r28
    lwz r6, 0x68(r1)
    li r5, 0x5
    mtctr r12
    bctrl
    b @L_801C0F0C
    @L_801C0EF8:
    lis r4, lbl_802757A0@ha
    la r3, lbl_8047DF40(r2)
    addi r5, r4, lbl_802757A0@l
    li r4, 0x35e
    bl fn_80196D78
    @L_801C0F0C:
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
