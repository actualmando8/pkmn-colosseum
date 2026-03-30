    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r8, 0x1
    stw r0, 0x24(r1)
    stw r4, 0x8(r1)
    stw r5, 0xc(r1)
    lwz r4, 0x8(r1)
    stw r6, 0x10(r1)
    lwz r5, 0xc(r1)
    stw r7, 0x14(r1)
    lwz r6, 0x10(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    lwz r7, 0x14(r1)
    bl fn_8017EB6C
    lis r3, lbl_80453FEC@ha
    addi r3, r3, lbl_80453FEC@l
    lwz r0, 0x1c(r3)
    cmplw r0, r31
    bne @L_8017DF38
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    bne @L_8017DF0C
    addi r3, r31, 0x70
    bl fn_80167F28
    stw r3, 0x68(r31)
    @L_8017DF0C:
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    beq @L_8017DF30
    lis r4, fn_8017F108@ha
    lwz r3, 0x68(r31)
    addi r5, r4, fn_8017F108@l
    li r4, 0x0
    bl fn_80167DD8
    b @L_8017DF38
    @L_8017DF30:
    mr r3, r31
    bl fn_8017D92C
    @L_8017DF38:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
