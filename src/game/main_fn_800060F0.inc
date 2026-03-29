    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r27, 0x7c(r1)
    mr r28, r3
    mr r27, r4
    bne cr1, @L_8000612C
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_8000612C:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    addi r6, r1, 0x98
    addi r0, r1, 0x8
    lis r4, 0x300
    lis r3, lbl_803A0700@ha
    lis r8, lbl_80266448@ha
    stw r4, 0x68(r1)
    addi r7, r1, 0x68
    mr r4, r5
    stw r6, 0x6c(r1)
    addi r3, r3, lbl_803A0700@l
    mr r5, r7
    addi r30, r8, lbl_80266448@l
    stw r0, 0x70(r1)
    bl fn_800C8600
    lis r4, lbl_803A0700@ha
    mr r31, r3
    addi r0, r4, lbl_803A0700@l
    mr r5, r28
    mr r6, r27
    addi r4, r30, 0x178
    add r3, r0, r31
    crxor 6, 6, 6
    bl fn_800C8520
    lis r4, lbl_803A0700@ha
    add r27, r31, r3
    addi r0, r4, lbl_803A0700@l
    addi r4, r30, 0x190
    add r3, r0, r27
    crxor 6, 6, 6
    bl fn_800C8520
    add r27, r27, r3
    li r29, 0x0
    bl fn_8009BD28
    mr r28, r3
    lis r3, lbl_803A0700@ha
    addi r31, r3, lbl_803A0700@l
    b @L_80006204
    @L_800061E0:
    lwz r6, 0x0(r28)
    mr r5, r28
    lwz r7, 0x4(r28)
    add r3, r31, r27
    addi r4, r30, 0x1b4
    crxor 6, 6, 6
    bl fn_800C8520
    add r27, r27, r3
    lwz r28, 0x0(r28)
    @L_80006204:
    cmplwi r28, 0x0
    beq @L_80006224
    addis r0, r28, 0x1
    cmplwi r0, 0xffff
    beq @L_80006224
    cmplwi r29, 0x20
    addi r29, r29, 0x1
    blt @L_800061E0
    @L_80006224:
    lis r4, lbl_803A0700@ha
    la r3, lbl_8047B6B8(r2)
    addi r5, r4, lbl_803A0700@l
    li r4, 0x196
    crxor 6, 6, 6
    bl fn_800060F0
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
