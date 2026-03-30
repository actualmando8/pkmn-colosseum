    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r27, 0x7c(r1)
    mr r27, r3
    mr r28, r5
    mr r29, r6
    mr r30, r7
    bne cr1, @L_800F735C
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_800F735C:
    addi r11, r1, 0x98
    addi r0, r1, 0x8
    lis r12, 0x600
    stw r3, 0x8(r1)
    addi r31, r1, 0x68
    mr r3, r4
    stw r4, 0xc(r1)
    mr r4, r8
    stw r5, 0x10(r1)
    mr r5, r31
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    stw r12, 0x68(r1)
    stw r11, 0x6c(r1)
    stw r0, 0x70(r1)
    bl fn_800F6D18
    mr r31, r3
    bl fn_800FF560
    cmplwi r31, 0x0
    mr r4, r3
    bne @L_800F73C4
    li r3, 0x0
    b @L_800F7420
    @L_800F73C4:
    lis r5, fn_800F6BC4@ha
    mr r3, r27
    addi r8, r5, fn_800F6BC4@l
    mr r7, r29
    mr r5, r28
    li r6, 0x1
    bl fn_800F07A8
    cmplwi r3, 0x0
    beq @L_800F7404
    stw r3, 0xc(r31)
    mr r5, r31
    li r4, 0x1
    stw r30, 0x10(r31)
    crxor 6, 6, 6
    bl fn_800F0654
    b @L_800F741C
    @L_800F7404:
    lis r3, lbl_80271294@ha
    lis r4, lbl_80315668@ha
    addi r3, r3, lbl_80271294@l
    addi r4, r4, lbl_80315668@l
    crxor 6, 6, 6
    bl fn_800DD970
    @L_800F741C:
    lhz r3, 0x6(r31)
    @L_800F7420:
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
