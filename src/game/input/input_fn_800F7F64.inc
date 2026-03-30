    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_80401C10@ha
    li r5, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r4, lbl_80401C10@l
    stw r29, 0x14(r1)
    lwz r0, 0x0(r30)
    cmpw r0, r3
    bne @L_800F7F98
    b @L_800F7FCC
    @L_800F7F98:
    lwzu r0, 0x6c(r30)
    cmpw r0, r3
    bne @L_800F7FA8
    b @L_800F7FCC
    @L_800F7FA8:
    lwzu r0, 0x6c(r30)
    cmpw r0, r3
    bne @L_800F7FB8
    b @L_800F7FCC
    @L_800F7FB8:
    lwzu r0, 0x6c(r30)
    cmpw r0, r3
    bne @L_800F7FC8
    b @L_800F7FCC
    @L_800F7FC8:
    mr r30, r5
    @L_800F7FCC:
    cmplwi r30, 0x0
    bne @L_800F7FDC
    li r3, 0x2
    b @L_800F8094
    @L_800F7FDC:
    lhz r0, 0x24(r30)
    stw r0, 0x30(r30)
    bl OSDisableInterrupts
    mr r29, r3
    bl fn_800D3094
    lwz r0, lbl_8047AC4C(r13)
    mr r31, r3
    cmplwi r0, 0x0
    beq @L_800F8018
    lwz r0, lbl_8047AC50(r13)
    cmplw r31, r0
    beq @L_800F8018
    lwz r3, lbl_8047AC4C(r13)
    bl fn_800AAD34
    stw r31, lbl_8047AC50(r13)
    @L_800F8018:
    addi r3, r30, 0x24
    addi r4, r30, 0x18
    li r5, 0xc
    bl memcpy
    mr r3, r29
    bl OSRestoreInterrupts
    addi r0, r30, 0x4c
    mr r3, r30
    stw r0, 0x8(r1)
    addi r6, r30, 0x34
    addi r7, r30, 0x35
    addi r8, r30, 0x38
    lbz r4, 0x26(r30)
    addi r9, r30, 0x3c
    lbz r5, 0x27(r30)
    addi r10, r30, 0x48
    bl fn_800F8654
    addi r0, r30, 0x54
    mr r3, r30
    stw r0, 0x8(r1)
    addi r6, r30, 0x36
    addi r7, r30, 0x37
    addi r8, r30, 0x40
    lbz r4, 0x28(r30)
    addi r9, r30, 0x44
    lbz r5, 0x29(r30)
    addi r10, r30, 0x50
    bl fn_800F8654
    mr r3, r30
    bl fn_800F8A54
    lwz r3, 0xc(r30)
    @L_800F8094:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
