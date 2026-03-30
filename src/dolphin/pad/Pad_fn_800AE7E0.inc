    nofralloc
    mflr r0
    lis r3, lbl_803125E8@ha
    stw r0, 0x4(r1)
    addi r5, r3, lbl_803125E8@l
    crxor 6, 6, 6
    addi r3, r5, 0x48
    stwu r1, -0x10(r1)
    addi r4, r5, 0x68
    stw r31, 0xc(r1)
    addi r5, r5, 0x74
    bl fn_800AE9AC
    lwz r0, lbl_8047A950(r13)
    cmpwi r0, 0x1
    beq @L_800AE890
    lwz r3, lbl_80478A40(r13)
    bl OSRegisterVersion
    bl OSDisableInterrupts
    lis r4, fn_800AE9FC@ha
    addi r31, r3, 0x0
    addi r4, r4, fn_800AE9FC@l
    li r3, 0x7
    bl __OSSetInterruptHandler
    lis r3, 0x100
    bl __OSUnmaskInterrupts
    lis r3, 0xcc00
    addi r6, r3, 0x5000
    lhz r3, 0x500a(r3)
    li r0, -0xa9
    and r0, r3, r0
    ori r0, r0, 0x800
    sth r0, 0xa(r6)
    li r5, -0xad
    li r4, 0x0
    lhz r7, 0xa(r6)
    li r0, 0x1
    addi r3, r31, 0x0
    and r5, r7, r5
    sth r5, 0xa(r6)
    stw r4, lbl_8047A960(r13)
    stw r4, lbl_8047A96C(r13)
    stw r4, lbl_8047A964(r13)
    stw r4, lbl_8047A968(r13)
    stw r0, lbl_8047A950(r13)
    bl OSRestoreInterrupts
    @L_800AE890:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr
